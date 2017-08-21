// user table definitions

#ifndef H_USER_TABLES
#define H_USER_TABLES

#include "Sys.h"
#include "TableBase.h"

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
