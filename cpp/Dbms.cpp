// Database Management System code

#include <cstring>
#include <mysql.h>
#include <my_global.h>
#include <stdio.h>
#include "Sys.h"
#include "Dbms.h"
#include "SysException.h"

void
DBConnection::RawConnection::close()
{
 if (This.value!=NULL) {
  mysql_close(This.value);
  This.value=NULL;
 } else {
  String exceptionText("Connection is not open");
  throw DBException(exceptionText);
 }
}

DBConnection::DBConnection()
{
 This.host="localhost";
 This.port=0;
 This.clientFlag=0;
}

void
DBConnection::connect()
{
 if (This.connectionPtr.getValue()!=NULL)
 {
  String exceptionText("Connection is open");
  throw DBException(exceptionText);
 }
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
 if (mysql_query(mysqlRawPtr,"USE CORE")) {
  String mysqlError(mysql_error(mysqlRawPtr));
  mysql_close(mysqlRawPtr);
  throw DBException(mysqlError);
 }
 if (mysql_query(mysqlRawPtr,"SET AUTOCOMMIT=0")) {
  String mysqlError(mysql_error(mysqlRawPtr));
  mysql_close(mysqlRawPtr);
  throw DBException(mysqlError);
 }
 This.connectionPtr.setValue(mysqlRawPtr);
}

void
DBConnection::close()
{
 This.connectionPtr.close();
}

void
DBConnection::startTransaction()
{
 MYSQL* mysqlRawPtr=This.connectionPtr.getValue();
 if (mysql_query(mysqlRawPtr,"START TRANSACTION")) {
  String mysqlError(mysql_error(mysqlRawPtr));
  This.connectionPtr.close();
  throw DBException(mysqlError);
 }
}

void
DBConnection::commit()
{
 MYSQL* mysqlRawPtr=This.connectionPtr.getValue();
 if (mysql_query(mysqlRawPtr,"COMMIT")) {
  String mysqlError(mysql_error(mysqlRawPtr));
  This.connectionPtr.close();
  throw DBException(mysqlError);
 }
}

void
DBConnection::rollback()
{
 MYSQL* mysqlRawPtr=This.connectionPtr.getValue();
 if (mysql_query(mysqlRawPtr,"ROLLBACK")) {
  String mysqlError(mysql_error(mysqlRawPtr));
  This.connectionPtr.close();
  throw DBException(mysqlError);
 }
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
DBStatement::addIdentifier(
 const DBIdentifier& tableName,
 const DBIdentifier& columnName)
{
 This.stmt.append(tableName);
 This.stmt.append(".");
 This.stmt.append(columnName);
}

void
DBStatement::addLiteral(const String& value)
{
 char query[1024];
 MYSQL* mysqlRawPtr=This.connection->connectionPtr.getValue();
 size_t len=mysql_real_escape_string(mysqlRawPtr,
  query,value.c_str(),value.size());
 This.stmt.append("'");
 This.stmt.append(query,len);
 This.stmt.append("'");
}

void
DBStatement::addLiteral(const Serial& value)
{
 char query[1024];
 String valueText(value.toString());
 MYSQL* mysqlRawPtr=This.connection->connectionPtr.getValue();
 size_t len=mysql_real_escape_string(mysqlRawPtr,
  query,valueText.c_str(),valueText.size());
 This.stmt.append(query,len);
}

void
DBStatement::executeQuery(DBResultSet& rset)
{
 if (DBLOGMODE==1)
  printf("%s\n",This.getSQL().c_str());
 MYSQL* connection=This.connection->connectionPtr.getValue();
 if (mysql_query(connection,This.getSQL().c_str()))
 {
  String mysqlError(mysql_error(connection));
  throw DBException(mysqlError);
 }
 rset.storeResult(This.connection); 
}

DBResultSet::DBResultSet(DBStatement& stmt)
{
 This.stmtPtr=&stmt;
 This.rsetPtr=NULL;
 This.columnCount=0;
}

DBResultSet::~DBResultSet()
{
 mysql_free_result(This.rsetPtr);
}

void
DBResultSet::storeResult(DBConnection* connection)
{
 This.rsetPtr=mysql_store_result(connection->connectionPtr.getValue());
 if (This.rsetPtr==NULL) {
  String mysqlError(mysql_error(connection->connectionPtr.getValue()));
  if (mysqlError.length()>0) {
   connection->close();
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
 This.columnCount=(size_t)mysql_num_fields(This.rsetPtr);
 return(This.currRow!=NULL);
}

void
DBResultSet::get(size_t pos,String& value)
{
 if (pos>=This.columnCount)
  throw DBException("Column Index out of bounds");
 const char* cPtr=This.currRow[pos];
 if (cPtr!=NULL)
 {
  size_t cLen=strlen(cPtr);
  value.assign(cPtr,cLen);
 }
 else
  value.clear();
}
 
void
DBResultSet::get(size_t pos,Serial& value)
{
 if (pos>=This.columnCount)
  throw DBException("Column Index out of bounds");
 const char* cPtr=This.currRow[pos];
 if (cPtr!=NULL)
 {
  size_t cLen=strlen(cPtr);
  String text(cPtr,cLen);
  value.fromString(text);
 }
 else
  value.setNull();
}
