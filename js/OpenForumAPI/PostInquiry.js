var net=require('net');
var XMLParser=require('pixl-xml');
var XMLBuilder=require('xmlbuilder');

var QueryRequest={};

/* Forum Inquiry */

QueryRequest.generateForumInquiry=function() {
 var xmlObject={
  OpenForumQuery: {
   Request: {
    ForumInquiry: null
 }}};
 return XMLBuilder.create(xmlObject).end();
};

QueryRequest.processForumInquiry=function(xml,callback) {
 if (xml.ForumInquiry===undefined)
  throw 'ForumInquiry not found';

 var ForumInquiry=xml.ForumInquiry;
 if (ForumInquiry.Boards===undefined)
  throw 'Boards not found';

 var Boards=ForumInquiry.Boards;
 if (Boards.Board===undefined)
  throw 'Board not found';

 var Board=Boards.Board;
 if (!Array.isArray(Board))
  Board=[Board];
 callback(Board);
};

/* Board Inquiry */

QueryRequest.generateBoardInquiry=function(serial) {
 var xmlObject={
  OpenForumQuery: {
   Request: {
    BoardInquiry: {
     Serial: serial
 }}}};
 return XMLBuilder.create(xmlObject).end();
};

QueryRequest.processBoardInquiry=function(xml,callback) {
 if (xml.BoardInquiry===undefined)
  throw 'BoardInquiry not found';
 console.log(JSON.stringify(xml.BoardInquiry));
 callback(xml.BoardInquiry);
};

/* Topic Inquiry */

QueryRequest.generateTopicInquiry=function(serial) {
 var xmlObject={
  OpenForumQuery: {
   Request: {
    TopicInquiry: {
     Serial: serial
 }}}};
 return XMLBuilder.create(xmlObject).end();
};

QueryRequest.processTopicInquiry=function(xml,callback) {
 if (xml.TopicInquiry===undefined)
  throw 'TopicInquiry not found';
 if (!Array.isArray(xml.TopicInquiry.Posts.Post))
  xml.TopicInquiry.Posts.Post=[xml.TopicInquiry.Posts.Post];
 callback(xml.TopicInquiry);
};

/* User Inquiry */

QueryRequest.generateUserInquiry=function(serial) {
 var xmlObject={
  OpenForumQuery: {
   Request: {
    UserInquiry:  {
     Serial: serial
 }}}};
 return XMLBuilder.create(xmlObject).end();
};

QueryRequest.processUserInquiry=function(xml,callback) {
 if (xml.UserInquiry===undefined)
  throw 'UserInquiry not found';
 callback(xml.UserInquiry);
};

var getForumListing=function(type,arguments,callback) {
 const client=net.createConnection({ port: 8081 },()=>{
  if (type==='ForumInquiry')
   client.write(QueryRequest.generateForumInquiry(arguments));
  else if (type==='BoardInquiry')
   client.write(QueryRequest.generateBoardInquiry(arguments));
  else if (type==='TopicInquiry')
   client.write(QueryRequest.generateTopicInquiry(arguments));
  else
   throw 'Unknown query type';
 });

 client.on('data',(data)=>{
  try {
   var xml=XMLParser.parse(data.toString());
 
   if (type==='ForumInquiry')
    QueryRequest.processForumInquiry(xml,callback);
   else if (type==='BoardInquiry')
    QueryRequest.processBoardInquiry(xml,callback);
   else if (type==='TopicInquiry')
    QueryRequest.processTopicInquiry(xml,callback);
   else
    throw 'Unknown query type';
  } catch (e) {
   console.log(e);
  }
 });
}

module.exports = {
 getForumListing: getForumListing
};
