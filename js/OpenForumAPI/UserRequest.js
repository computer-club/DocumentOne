var net=require('net');
var XMLParser=require('pixl-xml');
var XMLBuilder=require('xmlbuilder');

var QueryRequest={};

/* Forum Inquiry */

QueryRequest.generateLoginRequest=function(arguments) {
 var xmlObject={
  OpenForumQuery: {
   Request: {
    UserLoginVerify: {
     Username: arguments.Username,
     Password: arguments.Password
 }}}};
 return XMLBuilder.create(xmlObject).end();
};

QueryRequest.processLoginRequest=function(xml,callback) {
 // !!!
 console.log(JSON.stringify(xml));
 if (xml.UserLoginVerify===undefined)
  throw 'UserLoginVerify not found';

 var UserLoginVerify=xml.UserLoginVerify;

 if (UserLoginVerify.success==="N")
  throw 'Username or Password incorrect';
 if (UserLoginVerify.LoginToken===undefined)
  throw 'LoginToken not found';

 callback(UserLoginVerify.LoginToken);
};

QueryRequest.generateLogoutRequest=function(arguments) {
 var xmlObject={
  OpenForumQuery: {
   Request: {
    UserLogout: {
     LoginToken: arguments.LoginToken
 }}}};
 return XMLBuilder.create(xmlObject).end();
};

QueryRequest.processLogoutRequest=function(xml,callback) {
 if (xml.UserLogout===undefined)
  callback(null,'UserLogout not found: ' + JSON.stringify(xml));
 callback(xml.UserLogout);
};

QueryRequest.generateRegisterRequest=function(arguments) {
 const username=arguments.Username;
 const password=arguments.Password;
 if (username===undefined || password===undefined)
  throw 'Both Username and Password required';

 var xmlObject={
  OpenForumQuery: {
   Request: {
    UserRegister: {
     Username: username,
     Password: password
 }}}};
 return XMLBuilder.create(xmlObject).end();
};

QueryRequest.processRegisterRequest=function(xml,callback) {
 console.log(JSON.stringify(xml));
 if (xml.UserRegister===undefined)
  throw 'UserRegister not found';

 var UserRegister=xml.UserRegister;
 if (UserRegister.LoginToken===undefined)
  throw 'LoginToken not found';

 callback(UserRegister.LoginToken);
};

var doLoginAction=function(type,arguments,callback) {
 const client=net.createConnection({ port: 8081 },()=>{
  if (type==='LoginVerify')
   client.write(QueryRequest.generateLoginRequest(arguments));
  else if (type==='Logout')
   client.write(QueryRequest.generateLogoutRequest(arguments));
  else if (type==='Register')
   client.write(QueryRequest.generateRegisterRequest(arguments));
  else
   throw 'Unknown query type';
 });

 client.on('data',(data)=>{
  try {
   var xml=XMLParser.parse(data.toString());
 
   if (type==='LoginVerify')
    QueryRequest.processLoginRequest(xml,callback);
   else if (type==='Logout')
    QueryRequest.processLogoutRequest(xml,callback);
   else if (type==='Register')
    QueryRequest.processRegisterRequest(xml,callback);
   else
    throw 'Unknown query type';
  } catch (e) {
   console.log(e);
   throw e.message;
  }
 });
}

module.exports = {
 doLoginAction: doLoginAction
};
