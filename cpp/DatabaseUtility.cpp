// SQL Test Code

#include <stdio.h>
#include "Sys.h"
#include "Dbms.h"
#include "UserTables.h"

const String databaseName("CORE");

int
main()
{
 DBConnection connection;
 connection.connect();

 bool databaseExists=false;
 {
  DBStatement stmt(connection);
  DBResultSet rset(stmt);
  stmt.setSQL("SHOW DATABASES");
  stmt.executeQuery(rset);
  while (!databaseExists && rset.fetch()) {
   String currentDatabaseName;
   rset.get(0,currentDatabaseName);
   if (currentDatabaseName==databaseName)
    databaseExists=true;
  }
 }
 if (!databaseExists)
 {
  DBStatement stmt(connection);
  DBResultSet rset(stmt);
  String statement("CREATE DATABASE ");
  statement.append(databaseName);
  stmt.setSQL(statement);
  stmt.executeQuery(rset);
 }
 {
  UserTable userTable;
  String tableName(userTable.getName());
  bool tableExists;
  {
   DBStatement stmt(connection);
   DBResultSet rset(stmt);
   String stmtSQL("SHOW TABLES FROM ");
   stmtSQL.append(databaseName);
   stmt.setSQL(stmtSQL.c_str());
   stmt.executeQuery(rset);
   while (!tableExists && rset.fetch()) {
    String rowTableName;
    rset.get(0,rowTableName);
    if (tableName==rowTableName)
     tableExists=true;
   }
  }
  DBTable::DBColumnList::iterator begin=userTable.getColumnListBegin();
  DBTable::DBColumnList::iterator end=userTable.getColumnListEnd();
  String buildTableString;
  if (!tableExists) {
   buildTableString.assign("CREATE TABLE ");
   buildTableString.append(databaseName);
   buildTableString.append(".");
   buildTableString.append(tableName);
   buildTableString.append(" (");
  }
  bool first=true;
  while (begin!=end) {
   const DBColumn& column=**begin;
   String columnName=column.getColumnName();
   if (!tableExists) {
    if (first)
     first=false;
    else
     buildTableString.append(", ");
    buildTableString.append(columnName);
    buildTableString.append(" ");
    DBColumn::ColumnType columnType=column.getColumnType();
    String columnTypeString;
    if (columnType==DBColumn::ColumnType::String) {
     columnTypeString.assign("VARCHAR(");
     uint maxLength=((StringColumn*)&column)->getMaxLength();
     columnTypeString.append(std::to_string(maxLength));
     columnTypeString.append(")");
    }
    else if (columnType==DBColumn::ColumnType::Long) {
     columnTypeString.assign("BIGINT");
     bool autoIncrement=((SerialColumn*)&column)->getAutoIncrement();
     bool notNull=((SerialColumn*)&column)->getNotNull();
     if (notNull)
      columnTypeString.append(" NOT NULL");
     if (autoIncrement)
      columnTypeString.append(" AUTO_INCREMENT");
    }
    if (column.getIndexType()==DBColumn::IndexType::Primary)
     columnTypeString.append(" PRIMARY KEY");
    buildTableString.append(columnTypeString);
   } else {
    DBStatement stmt(connection);
    DBResultSet rset(stmt);
    String sqlStatement("SHOW TABLES FROM ");
    sqlStatement.append(databaseName);
    stmt.setSQL(sqlStatement);
    stmt.executeQuery(rset);
    bool found=false;
    while (!found && rset.fetch()) {
     String rowTableName;
     rset.get(0,rowTableName);
     if (rowTableName==tableName)
      found=true;
    }
    if (!found) {
     String sqlStatement("ALTER TABLE ");
     sqlStatement.append(databaseName);
     sqlStatement.append(".");
     sqlStatement.append(tableName);
     sqlStatement.append(" ADD ");
     DBColumn::ColumnType columnType=column.getColumnType();
     String columnTypeString;
     if (columnType==DBColumn::ColumnType::String) {
      columnTypeString.assign("VARCHAR(");
      columnTypeString.append(((StringColumn*)&column)->getMaxLength());
      columnTypeString.append(")");
     }
     else if (columnType==DBColumn::ColumnType::Long) {
      columnTypeString.assign("BIGINT");
      bool autoIncrement=((SerialColumn*)&column)->getAutoIncrement();
      bool notNull=((SerialColumn*)&column)->getNotNull();
      if (notNull)
       columnTypeString.append(" NOT NULL");
      if (autoIncrement)
       columnTypeString.append(" AUTO_INCREMENT");
     }
     if (column.getIndexType()==DBColumn::IndexType::Primary)
      columnTypeString.append(" PRIMARY KEY");
     sqlStatement.append(columnTypeString);
     DBStatement stmt(connection);
     DBResultSet rset(stmt);
     stmt.setSQL(sqlStatement);
     stmt.executeQuery(rset);
    }
   }
   ++begin;
  }
  if (!tableExists) {
   buildTableString.append(")");
   DBStatement stmt(connection);
   DBResultSet rset(stmt);
   stmt.setSQL(buildTableString);
   stmt.executeQuery(rset);
  }
 }
 return 0;
}
