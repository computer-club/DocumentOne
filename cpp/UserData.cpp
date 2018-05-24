// user data object code

#include <ctime>
#include <iomanip>
#include "Sys.h"
#include "Dbms.h"
#include "UserData.h"

void
UserData::retrieve(
 DBConnection& connection,
 const Serial& serial)
{
 This.connectionPtr=&connection;
 DBStatement stmt(connection);
 DBResultSet rset(stmt);
 stmt.setSelect();
 stmt.addIdentifier(
  UserTable::getSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  UserTable::getUsernameColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  UserTable::getRoleSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  UserTable::getTokenColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  UserTable::getTokenExpirationDateColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  UserTable::getTokenExpirationTimeColumnName());
 stmt.addFrom();
 stmt.addIdentifier(UserTable::getName());
 stmt.addWhere();
 stmt.addIdentifier(
  UserTable::getSerialColumnName());
 stmt.addIsEqualTo();
 stmt.addLiteral(serial);
 stmt.executeQuery(rset);
 if (rset.fetch())
 {
  This.isRetrieved=true;
  size_t pos=0;
  rset.get(pos++,This.serial);
  rset.get(pos++,This.username);
  rset.get(pos++,This.roleSerial);
  rset.get(pos++,This.token);
  rset.get(pos++,This.tokenExpirationDate);
  rset.get(pos++,This.tokenExpirationTime);
 }
 else
  throw SysException("UserData retrieve failed");
}

void
UserData::update()
{
 if (!This.isRetrieved)
  throw SysException("UserData not retrieved");
 DBConnection& connection=*This.connectionPtr;
 DBStatement stmt(connection);
 DBResultSet rset(stmt);
 stmt.setUpdate();
 stmt.addIdentifier(UserTable::getName());
 stmt.addSet();
 stmt.addIdentifier(UserTable::getUsernameColumnName());
 stmt.addIsEqualTo();
 stmt.addLiteral(This.username);
 if (!This.token.empty())
 {
  stmt.addComma();
  stmt.addIdentifier(UserTable::getTokenColumnName());
  stmt.addIsEqualTo();
  stmt.addLiteral(This.token);
  stmt.addComma();
  stmt.addIdentifier(UserTable::getTokenExpirationDateColumnName());
  stmt.addIsEqualTo();
  stmt.addLiteral(This.tokenExpirationDate);
  stmt.addComma();
  stmt.addIdentifier(UserTable::getTokenExpirationTimeColumnName());
  stmt.addIsEqualTo();
  stmt.addLiteral(This.tokenExpirationTime);
 }
 stmt.addWhere();
 stmt.addIdentifier(UserTable::getSerialColumnName());
 stmt.addIsEqualTo();
 stmt.addLiteral(This.serial);
 stmt.executeQuery(rset);
}

bool
UserData::verifyPassword(const String& passwordString)
{
 bool valid=false;
 if (This.isRetrieved)
 {
  DBConnection& connection=*This.connectionPtr;
  DBStatement stmt(connection);
  DBResultSet rset(stmt);
  stmt.setSelect();
  stmt.addIdentifier(UserTable::getPasswordColumnName());
  stmt.addFrom();
  stmt.addIdentifier(UserTable::getName());
  stmt.addWhere();
  stmt.addIdentifier(UserTable::getSerialColumnName());
  stmt.addIsEqualTo();
  stmt.addIdentifier(This.serial.toString());
  stmt.executeQuery(rset);
  if (rset.fetch())
  {
   String password;
   rset.get(0,password);
   if (password==passwordString)
    valid=true;
  }
 }
 return(valid);
}

bool
UserData::performLogin(
 DBConnection& connection,
 const String& username,
 const String& password)
{
 bool valid=false;
 Serial userSerial;
 {
  DBStatement stmt(connection);
  DBResultSet rset(stmt);
  stmt.setSelect();
  stmt.addIdentifier(UserTable::getSerialColumnName());
  stmt.addFrom();
  stmt.addIdentifier(UserTable::getName());
  stmt.addWhere();
  stmt.addIdentifier(UserTable::getUsernameColumnName());
  stmt.addIsEqualTo();
  stmt.addLiteral(username);
  stmt.executeQuery(rset);
  if (rset.fetch())
   rset.get(0,userSerial);
 }
 if (!userSerial.isNull())
 {
  try {
   This.retrieve(connection,userSerial);
   if (This.verifyPassword(password))
    valid=true;
   if (valid && !This.validateTokenNonExpired(
        This.tokenExpirationDate,This.tokenExpirationTime))
   {
    This.generateNewToken();
    This.update();
   }
  }
  catch (...)
  {}
 }
 return(valid);
}

bool
UserData::performRegister(
 DBConnection& connection,
 const String& username,
 const String& password)
{
 bool valid=true;
 if (username.empty() || password.empty())
  valid=false;
 Serial userSerial;
 if (valid)
 {
  DBStatement stmt(connection);
  DBResultSet rset(stmt);
  stmt.setSelect();
  stmt.addIdentifier(UserTable::getSerialColumnName());
  stmt.addFrom();
  stmt.addIdentifier(UserTable::getName());
  stmt.addWhere();
  stmt.addIdentifier(UserTable::getUsernameColumnName());
  stmt.addIsEqualTo();
  stmt.addLiteral(username);
  stmt.executeQuery(rset);
  if (rset.fetch())
   rset.get(0,userSerial);
 }
 if (valid && userSerial.isNull())
 {
  valid=false;
  This.generateNewToken();
  DBStatement stmt(connection);
  DBResultSet rset(stmt);

  stmt.setInsertInto();
  stmt.addIdentifier(UserTable::getName());
  stmt.addLeftParenthesis();
  stmt.addIdentifier(UserTable::getUsernameColumnName());
  stmt.addComma();
  stmt.addIdentifier(UserTable::getPasswordColumnName());
  stmt.addComma();
  stmt.addIdentifier(UserTable::getTokenColumnName());
  stmt.addComma();
  stmt.addIdentifier(UserTable::getTokenExpirationDateColumnName());
  stmt.addComma();
  stmt.addIdentifier(UserTable::getTokenExpirationTimeColumnName());
  stmt.addRightParenthesis();
  stmt.addValues();
  stmt.addLeftParenthesis();
  stmt.addLiteral(username);
  stmt.addComma();
  stmt.addLiteral(password);
  stmt.addComma();
  stmt.addLiteral(This.token);
  stmt.addComma();
  stmt.addLiteral(This.tokenExpirationDate);
  stmt.addComma();
  stmt.addLiteral(This.tokenExpirationTime);
  stmt.addRightParenthesis();
  stmt.executeQuery(rset);

  stmt.setSelect();
  stmt.addLastInsertId();
  stmt.executeQuery(rset);
  if (rset.fetch())
  {
   rset.get(0,userSerial);
   try {
    This.retrieve(connection,userSerial);
    valid=true;
   }
   catch (...)
   {}
  }
 }
 return(valid);
}

bool
UserData::performLogout(
 DBConnection& connection,
 const String& token)
{
 bool valid=false;
 Serial userSerial=This.getUserSerialByToken(connection,token);
 if (!userSerial.isNull())
 {
  try {
   This.retrieve(connection,userSerial);
   if (This.validateTokenNonExpired(
        This.tokenExpirationDate,This.tokenExpirationTime))
   {
    This.tokenExpirationDate.clear();
    This.tokenExpirationTime.clear();
    This.update();
    valid=true;
   }
  }
  catch (...)
  {}
 }
 return(valid);
}

void
UserData::generateNewToken()
{
 time_t timeNow;
 {
  time(&timeNow);
  char buffer[64];
  size_t size=snprintf(buffer,sizeof(buffer),"%X",timeNow);
  This.token.assign(buffer,size);
 }
 timeNow=timeNow+21600; // Six hours
 struct tm* expirationTimeInfo=localtime(&timeNow);
 char buffer[16];
 size_t size;
 size=strftime(buffer,sizeof(buffer),"%Y-%m-%d",expirationTimeInfo);
 This.tokenExpirationDate.assign(buffer,size);
 size=strftime(buffer,sizeof(buffer),"%H:%M:%S",expirationTimeInfo);
 This.tokenExpirationTime.assign(buffer,size);
}

bool
UserData::validateTokenNonExpired(
 const String& expirationDate,
 const String& expirationTime)
{
 bool result=true;
 String expirationDateTime(expirationDate);
 expirationDateTime.append(expirationTime);
 time_t timeNow;
 time(&timeNow);
 struct tm expirationTimeInfo;
 memset(&expirationTimeInfo,0,sizeof(struct tm));
 if (strptime(expirationDateTime.c_str(),"%Y-%m-%d %H:%M:%S",
      &expirationTimeInfo)!=NULL)
 {
  int secondsToExpiration=(int)difftime(mktime(&expirationTimeInfo),timeNow);
  if (secondsToExpiration<0)
   result=false;
 }
 else
  result=false;
 return(result);
}

const Serial
UserData::getUserSerialByToken(
 DBConnection& connection,
 const String& token)
{
 Serial result;
 DBStatement stmt(connection);
 DBResultSet rset(stmt);
 stmt.setSelect();
 stmt.addIdentifier(UserTable::getSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(UserTable::getTokenExpirationDateColumnName());
 stmt.addComma();
 stmt.addIdentifier(UserTable::getTokenExpirationTimeColumnName());
 stmt.addFrom();
 stmt.addIdentifier(UserTable::getName());
 stmt.addWhere();
 stmt.addIdentifier(UserTable::getTokenColumnName());
 stmt.addIsEqualTo();
 stmt.addLiteral(token);
 stmt.executeQuery(rset);
 if (rset.fetch())
 {
  Serial serial;
  String expirationDate;
  String expirationTime;
  size_t pos=0;
  rset.get(pos++,serial);
  rset.get(pos++,expirationDate);
  rset.get(pos++,expirationTime);
  if (UserData::validateTokenNonExpired(expirationDate,expirationTime))
   result=serial;
 }
 return(result);
}

bool
UserData::checkRestrictedAccess(
 DBConnection& connection,
 Permission::Category category)
{
 if (!This.isRetrieved)
  throw SysException("UserData not retrieved");
 bool allowed=false;
 bool found=false;
 DBStatement stmt(connection);
 DBResultSet rset(stmt);

 stmt.setSelect();
 stmt.addIdentifier(
  PermissionOverrideTable::getTypeColumnName());
 stmt.addFrom();
 stmt.addIdentifier(PermissionOverrideTable::getName());
 stmt.addWhere();
 stmt.addIdentifier(
  PermissionOverrideTable::getParentSerialColumnName());
 stmt.addIsEqualTo();
 stmt.addLiteral(This.serial);
 stmt.addAnd();
 stmt.addIdentifier(
  PermissionOverrideTable::getCategoryColumnName());
 stmt.addIsEqualTo();
 stmt.addLiteral(category.toString());
 stmt.executeQuery(rset);
 if (rset.fetch())
 {
  found=true;
  String typeString;
  rset.get(0,typeString);
  Permission::Type type;
  type.fromString(typeString);
  if (type==Permission::Type::Allowed ||
      type==Permission::Type::AlwaysAllowed)
   allowed=true;
 }

 if (!found)
 {
  stmt.setSelect();
  stmt.addIdentifier(
   PermissionTable::getTypeColumnName());
  stmt.addFrom();
  stmt.addIdentifier(PermissionTable::getName());
  stmt.addWhere();
  stmt.addIdentifier(
   PermissionTable::getParentSerialColumnName());
  stmt.addIsEqualTo();
  stmt.addLiteral(This.roleSerial);
  stmt.addAnd();
  stmt.addIdentifier(
   PermissionTable::getCategoryColumnName());
  stmt.addIsEqualTo();
  stmt.addLiteral(category.toString());
  stmt.executeQuery(rset);
  if (rset.fetch())
  {
   found=true;
   String typeString;
   rset.get(0,typeString);
   Permission::Type type;
   type.fromString(typeString);
   if (type==Permission::Type::Allowed ||
       type==Permission::Type::AlwaysAllowed)
    allowed=true;
  }
 }
 if (!found)
 {
  Permission::Type type(category.getDefault());
  if (type==Permission::Type::Allowed ||
      type==Permission::Type::AlwaysAllowed)
   allowed=true;
 }
 return(allowed);
}
