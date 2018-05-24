// User Register, Login, Logout Requests

#ifndef H_USER_CONTROL_REQUEST
#define H_USER_CONTROL_REQUEST

#include "Sys.h"
#include "QueryProcessor.h"
#include "UserData.h"

class UserLoginRequest: public QueryProcessorRequestBase
{
// private data
private:
 DBConnection* connectionPtr;
 UserData userData;
 BoolFalse success;

// public functions
public:
 UserLoginRequest(
  DBConnection& connection,
  QueryRequest& request);

 virtual void perform();
 virtual void generateXML(
  xmlDocPtr document);
};

class UserRegisterRequest: public QueryProcessorRequestBase
{
// private data
private:
 DBConnection* connectionPtr;
 UserData userData;

// public functions
public:
 UserRegisterRequest(
  DBConnection& connection,
  QueryRequest& request);

 virtual void perform();
 virtual void generateXML(
  xmlDocPtr document);
};

class UserLogoutRequest: public QueryProcessorRequestBase
{
// private data
private:
 DBConnection* connectionPtr;
 UserData userData;
 String token;

// public functions
public:
 UserLogoutRequest(
  DBConnection& connection,
  QueryRequest& request);

 virtual void perform();
 virtual void generateXML(
  xmlDocPtr document);
};

class UserPermissionRequest: public QueryProcessorRequestBase
{
// private data
private:
 DBConnection* connectionPtr;
 UserData userData;
 String token;
 String permission;

 bool accessAllowed;

// public functions
public:
 UserPermissionRequest(
  DBConnection& connection,
  QueryRequest& request);

 virtual void perform();
 virtual void generateXML(
  xmlDocPtr document);
};

#endif
