// Database Management System code

#include <cstring>
#include <mysql.h>
#include <my_global.h>
#include <stdio.h>
#include "Sys.h"
#include "Dbms.h"
#include "SysException.h"

DBConnection::DBConnection()
{
 This.host="localhost";
 This.port=0;
 This.clientFlag=0;
}

bool
DBConnection::connect()
{
 bool success=false;
 MYSQL* mysqlRawPtr=mysql_init(NULL);
 if (mysql_real_connect(mysqlRawPtr,This.host.c_str(),
      This.username.c_str(),This.password.c_str(),
      This.databaseName.c_str(),This.port,NULL,
      This.clientFlag)==NULL)
 {
  String mysqlError(mysql_error(mysqlRawPtr));
  mysql_close(mysqlRawPtr);
  throw DBException(mysqlError);
 }
 success=true;
 This.connectionPtr.setValue(mysqlRawPtr);
 return(success);
}

bool
DBConnection::RawConnection::setValue(MYSQL* ptr)
{
 if(This.value!=NULL)
  return false;
 This.value=ptr;
 return true;
}

void
DBStatement::executeQuery(DBResultSet& rset)
{
 MYSQL* connection=This.connection->connectionPtr.getValue();
 if (mysql_query(connection, This.getSQL().c_str())) {
  String mysqlError(mysql_error(connection));
  mysql_close(connection);
  throw DBException(mysqlError);
 }
 rset.storeResult(connection); 
}

DBResultSet::DBResultSet(DBStatement& stmt)
{
 This.stmtPtr=&stmt;
 This.rsetPtr=NULL;
}

DBResultSet::~DBResultSet()
{
 mysql_free_result(This.rsetPtr);
}

void
DBResultSet::storeResult(MYSQL* connection)
{
 This.rsetPtr=mysql_store_result(connection);
 if (This.rsetPtr==NULL) {
  String mysqlError(mysql_error(connection));
  if (mysqlError.length()>0) {
   mysql_close(connection);
   throw DBException(mysqlError);
  }
 }
}

bool
DBResultSet::fetch()
{
 if (This.rsetPtr==NULL) {
  return false;
 }
 This.currRow=mysql_fetch_row(This.rsetPtr);
 return (This.currRow!=NULL);
}

void
DBResultSet::get(size_t pos,String& value)
{
 const char* cPtr=This.currRow[pos];
 size_t cLen=strlen(cPtr);
 value.assign(cPtr,cLen);
}
