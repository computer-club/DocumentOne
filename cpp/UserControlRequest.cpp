// User Register, Login, Logout Requests

#include <libxml/tree.h>
#include "Sys.h"
#include "SysException.h"
#include "Dbms.h"
#include "UserControlRequest.h"

UserLoginRequest::UserLoginRequest(
 DBConnection& connection,
 QueryRequest& request)
{
 This.connectionPtr=&connection;
 String username;
 String password;
 QueryRequest::ElementList::const_iterator next(
  request.elementListBegin());
 QueryRequest::ElementList::const_iterator last(
  request.elementListEnd());
 while (next!=last)
 {
  String elementName(next->getName());
  String elementValue(next->getValue());
  if (elementName=="Username")
   username=elementValue;
  else if (elementName=="Password")
   password=elementValue;
  else
  {
   String text("Invalid request element '");
   text.append(elementName);
   text.append("'");
   throw SysException(text);
  }
  ++next;
 }
 This.success=This.userData.performLogin(connection,username,password);
}

void
UserLoginRequest::perform()
{
 // perform logic done in constructor to avoid keeping password around
}

void
UserLoginRequest::generateXML(xmlDocPtr document)
{
 UserData& userData=This.userData;
 String userSerial(userData.getSerial().toString());
 String username(userData.getUsername());
 String loginToken(userData.getLoginToken());

 xmlNode* rootElement=xmlDocGetRootElement(document);
 xmlNode* userElement=xmlNewChild(rootElement,NULL,
  BAD_CAST "UserLoginVerify",NULL);

 xmlNewChild(userElement,NULL,
  BAD_CAST "Success",
  BAD_CAST (This.success?"Y":"N"));

 if (!This.success)
  throw SysException("Invalid login");
 else
 {
  xmlNewChild(userElement,NULL,
   BAD_CAST "Serial",
   BAD_CAST userSerial.c_str());
  xmlNewChild(userElement,NULL,
   BAD_CAST "Username",
   BAD_CAST username.c_str());
  xmlNewChild(userElement,NULL,
   BAD_CAST "LoginToken",
   BAD_CAST loginToken.c_str());
 }
}

UserRegisterRequest::UserRegisterRequest(
 DBConnection& connection,
 QueryRequest& request)
{
 This.connectionPtr=&connection;
 String username;
 String password;
 QueryRequest::ElementList::const_iterator next(
  request.elementListBegin());
 QueryRequest::ElementList::const_iterator last(
  request.elementListEnd());
 while (next!=last)
 {
  String elementName(next->getName());
  String elementValue(next->getValue());
  if (elementName=="Username")
   username=elementValue;
  else if (elementName=="Password")
   password=elementValue;
  else
  {
   String text("Invalid request element '");
   text.append(elementName);
   text.append("'");
   throw SysException(text);
  }
  ++next;
 }
 if (username.empty() || password.empty())
 {
  String text("UserRegister failed: username and password ");
  text.append("must be specified");
  throw SysException(text);
 }
 else if (!This.userData.performRegister(connection,username,password))
  throw SysException("UserRegister: register failed");
}

void
UserRegisterRequest::perform()
{
 // perform logic done in constructor to avoid keeping password around
}

void
UserRegisterRequest::generateXML(xmlDocPtr document)
{
 UserData& userData=This.userData;
 String userSerial(userData.getSerial().toString());
 String username(userData.getUsername());
 String loginToken(userData.getLoginToken());

 xmlNode* rootElement=xmlDocGetRootElement(document);
 xmlNode* userElement=xmlNewChild(rootElement,NULL,
  BAD_CAST "UserRegister",NULL);

 xmlNewChild(userElement,NULL,
  BAD_CAST "Serial",
  BAD_CAST userSerial.c_str());
 xmlNewChild(userElement,NULL,
  BAD_CAST "Username",
  BAD_CAST username.c_str());
 xmlNewChild(userElement,NULL,
  BAD_CAST "LoginToken",
  BAD_CAST loginToken.c_str());
}

UserLogoutRequest::UserLogoutRequest(
 DBConnection& connection,
 QueryRequest& request)
{
 This.connectionPtr=&connection;
 QueryRequest::ElementList::const_iterator next(
  request.elementListBegin());
 QueryRequest::ElementList::const_iterator last(
  request.elementListEnd());
 while (next!=last)
 {
  String elementName(next->getName());
  String elementValue(next->getValue());
  if (elementName=="LoginToken")
   This.token=elementValue;
  else
  {
   String text("Invalid request element '");
   text.append(elementName);
   text.append("'");
   throw SysException(text);
  }
  ++next;
 }
 if (This.token.empty())
  throw SysException("No Login Token provided");
}

void
UserLogoutRequest::perform()
{
 DBConnection& connection=*This.connectionPtr;
 if (!This.userData.performLogout(connection,This.token))
  throw SysException("UserLogout failed");
}

void
UserLogoutRequest::generateXML(xmlDocPtr document)
{
 UserData& userData=This.userData;
 String userSerial(userData.getSerial().toString());
 String username(userData.getUsername());

 xmlNode* rootElement=xmlDocGetRootElement(document);
 xmlNode* userElement=xmlNewChild(rootElement,NULL,
  BAD_CAST "UserLogout",NULL);

 xmlNewChild(userElement,NULL,
  BAD_CAST "Serial",
  BAD_CAST userSerial.c_str());
 xmlNewChild(userElement,NULL,
  BAD_CAST "Username",
  BAD_CAST username.c_str());
}

UserPermissionRequest::UserPermissionRequest(
 DBConnection& connection,
 QueryRequest& request)
{
 This.connectionPtr=&connection;
 QueryRequest::ElementList::const_iterator next(
  request.elementListBegin());
 QueryRequest::ElementList::const_iterator last(
  request.elementListEnd());
 while (next!=last)
 {
  String elementName(next->getName());
  String elementValue(next->getValue());
  if (elementName=="Permission")
   This.permission=elementValue;
  else if (elementName=="LoginToken")
   This.token=elementValue;
  else
  {
   String text("Invalid request element '");
   text.append(elementName);
   text.append("'");
   throw SysException(text);
  }
  ++next;
 }
 if (This.token.empty())
  throw SysException("No Login Token provided");
 if (This.permission.empty())
  throw SysException("No Permission provided");
}

void
UserPermissionRequest::perform()
{
 DBConnection& connection=*This.connectionPtr;
 Permission::Category category;
 if (!category.fromString(This.permission))
  throw SysException("No value entry for string");
 This.userData.retrieve(connection,
  UserData::getUserSerialByToken(connection,This.token));
 This.accessAllowed=
  userData.checkRestrictedAccess(connection,category);
}

void
UserPermissionRequest::generateXML(xmlDocPtr document)
{
 UserData& userData=This.userData;
 String userSerial(userData.getSerial().toString());
 String username(userData.getUsername());
 String permission(This.permission);
 String result("No");
 if (This.accessAllowed);
  result="Yes";

 xmlNode* rootElement=xmlDocGetRootElement(document);
 xmlNode* userElement=xmlNewChild(rootElement,NULL,
  BAD_CAST "UserPermission",NULL);

 xmlNewChild(userElement,NULL,
  BAD_CAST "Serial",
  BAD_CAST userSerial.c_str());
 xmlNewChild(userElement,NULL,
  BAD_CAST "Username",
  BAD_CAST username.c_str());
 xmlNewChild(userElement,NULL,
  BAD_CAST "Permission",
  BAD_CAST permission.c_str());
 xmlNewChild(userElement,NULL,
  BAD_CAST "AccessAllowed",
  BAD_CAST result.c_str());
}
