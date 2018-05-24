// user class definitions

#ifndef H_USER
#define H_USER

#include "Sys.h"
#include "Serial.h"
#include "TableBase.h"

class User
{
// public types 
public:
 enum {MaxUsernameLength=40};
 enum {MaxPasswordLength=40};
 enum {MaxTokenLength=20};
};

class UserTable: public DBTable
{
// private columns
private:
 static const String name;
 static const String serialColumnName;
 static const String usernameColumnName;
 static const String passwordColumnName;
 static const String roleSerialColumnName;
 static const String tokenColumnName;
 static const String tokenExpirationDateColumnName;
 static const String tokenExpirationTimeColumnName;

// public functions
public:
 UserTable();

 static const String getName()
 { return(UserTable::name); }
 static const String getSerialColumnName()
 { return(UserTable::serialColumnName); }
 static const String getUsernameColumnName()
 { return(UserTable::usernameColumnName); }
 static const String getPasswordColumnName()
 { return(UserTable::passwordColumnName); }
 static const String getTokenColumnName()
 { return(UserTable::tokenColumnName); }
 static const String getTokenExpirationDateColumnName()
 { return(UserTable::tokenExpirationDateColumnName); }
 static const String getTokenExpirationTimeColumnName()
 { return(UserTable::tokenExpirationTimeColumnName); }
 static const String getRoleSerialColumnName()
 { return(UserTable::roleSerialColumnName); }
};

class Role
{
// public types 
public:
 enum {MaxDescriptionLength=40};
};

class RoleTable: public DBTable
{
// private columns
private:
 static const String name;
 static const String serialColumnName;
 static const String descriptionColumnName;

// public functions
public:
 RoleTable();

 static const String getName()
 { return(RoleTable::name); }
 static const String getSerialColumnName()
 { return(RoleTable::serialColumnName); }
 static const String getDescriptionColumnName()
 { return(RoleTable::descriptionColumnName); }
};

#endif
