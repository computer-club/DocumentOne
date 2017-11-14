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
};

class UserTable: public DBTable
{
// private columns
private:
 static const String name;
 static const String serialColumnName;
 static const String usernameColumnName;
 static const String passwordColumnName;

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
};

#endif
