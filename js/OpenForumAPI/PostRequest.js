var net=require('net');
var XMLParser=require('pixl-xml');
var XMLBuilder=require('xmlbuilder');

var QueryRequest={};

/* Forum Inquiry */

QueryRequest.generatePostRequest=function(arguments) {
 if (arguments.LoginToken===undefined)
  throw 'LoginToken required';
 if (arguments.TopicSerial===undefined)
  throw 'TopicSerial required';
 if (arguments.Subject===undefined)
  throw 'Subject required';
 if (arguments.Content===undefined)
  throw 'Content required';

 var xmlObject={
  OpenForumQuery: {
   Request: {
    UserPost: {
     LoginToken: arguments.LoginToken,
     TopicSerial: arguments.TopicSerial,
     Subject: arguments.Subject,
     Content: arguments.Content
 }}}};
 return XMLBuilder.create(xmlObject).end();
};

QueryRequest.processPostRequest=function(xml,callback) {
 if (xml.UserPost===undefined)
 {
  console.log(JSON.stringify(xml));
  throw 'UserPost not found';
 }

 var UserPost=xml.UserPost;
 if (UserPost.TopicSerial===undefined)
  throw 'TopicSerial not found';

 callback(UserPost);
};

var doPostAction=function(type,arguments,callback) {
 const client=net.createConnection({ port: 8081 },()=>{
  if (type==='UserPost')
   client.write(QueryRequest.generatePostRequest(arguments));
//else if (type==='PostDelete')
// client.write(QueryRequest.generateLogoutRequest(arguments));
  else
   throw 'Unknown query type';
 });

 client.on('data',(data)=>{
  try {
   var xml=XMLParser.parse(data.toString());
 
   if (type==='UserPost')
    QueryRequest.processPostRequest(xml,callback);
// else if (type==='Logout')
//  QueryRequest.processLogoutRequest(xml,callback);
   else
    throw 'Unknown query type';
  } catch (e) {
   console.log(e);
  }
 });
}

module.exports = {
 doPostAction: doPostAction
};
