// user data object definitions

#ifndef H_USER_DATA
#define H_USER_DATA

#include "Sys.h"
#include "User.h"

class DBConnection;

class UserData
{
// private data
private:
 DBConnection* connectionPtr;
 BoolFalse isRetrieved;
 Serial serial;
 String username;
 String token;
 String tokenExpirationDate;
 String tokenExpirationTime;

// public functions
public:
 UserData()
 {}
 UserData(DBConnection& connection)
 { This.connectionPtr=&connection; }

 void retrieve(
  DBConnection& connection,
  const Serial& serial);
 void update();

 bool verifyPassword(const String& passwordString);
 bool performLogin(
  DBConnection& connection,
  const String& username,
  const String& password);
 bool performRegister(
  DBConnection& connection,
  const String& username,
  const String& password);
 bool performLogout(
  DBConnection& connection,
  const String& token);

 const Serial& getSerial() const
 { return(This.serial); }

 const String& getUsername() const
 { return(This.username); }
 void setUsername(String& value)
 { This.username=value; }

 const String& getLoginToken() const
 { return(This.token); }
 const String& getTokenExpirationDate() const
 { return(This.tokenExpirationDate); }
 const String& getTokenExpirationTime() const
 { return(This.tokenExpirationTime); }

// public static functions
public:
 static const Serial getUserSerialByToken(
  DBConnection& connection,const String& token);
 static bool validateTokenNonExpired(
  const String& expirationDate,
  const String& expirationTime);

// private functions
private:
 void setToken(String& value)
 { This.token=value; }
 void setTokenExpirationDate(String& value)
 { This.tokenExpirationDate=value; }
 void setTokenExpirationTime(String& value)
 { This.tokenExpirationTime=value; }
 void generateNewToken();
};

#endif
