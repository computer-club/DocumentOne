// SQL Test Code

#include <stdio.h>
#include "Sys.h"
#include "Dbms.h"
#include "User.h"
#include "Permission.h"

const String databaseName("CORE");

String getColumnDefinitionString(
 const DBColumn& column,bool tableExists)
{
 String text(column.getColumnName());
 DBColumn::ColumnType columnType=column.getColumnType();
 if (columnType==DBColumn::ColumnType::String) {
  text.append(" VARCHAR(");
  uint maxLength=((StringColumn*)&column)->getMaxLength();
  text.append(std::to_string(maxLength));
  text.append(")");
 }
 else if (columnType==DBColumn::ColumnType::Long) {
  SerialColumn& serialColumn=*((SerialColumn*)&column);
  bool autoIncrement=serialColumn.getAutoIncrement();
  bool notNull=serialColumn.getNotNull();
  bool foreignKeyReference=serialColumn.getForeignKeyReference();
  text.append(" BIGINT");
  if (notNull)
   text.append(" NOT NULL");
  if (autoIncrement)
   text.append(" AUTO_INCREMENT");
  if (foreignKeyReference && !tableExists) {
   String foreignKeyReferenceTableName(
    serialColumn.getForeignKeyReferenceTableName());
   String foreignKeyReferenceColumnName(
    serialColumn.getForeignKeyReferenceColumnName());
   text.append(", FOREIGN KEY (");
   text.append(column.getColumnName());
   text.append(") REFERENCES ");
   text.append(databaseName);
   text.append(".");
   text.append(foreignKeyReferenceTableName);
   text.append("(");
   text.append(foreignKeyReferenceColumnName);
   text.append(")");
  }
  else if (column.getIndexType()==DBColumn::IndexType::Primary)
   text.append(" PRIMARY KEY");
 }
 else if (columnType==DBColumn::ColumnType::Date) {
  DateColumn& dateColumn=*((DateColumn*)&column);
  text.append(" DATE");
  bool notNull=dateColumn.getNotNull();
  if (notNull)
   text.append(" NOT NULL");
 }
 else if (columnType==DBColumn::ColumnType::Time) {
  TimeColumn& timeColumn=*((TimeColumn*)&column);
  text.append(" TIME");
  bool notNull=timeColumn.getNotNull();
  if (notNull)
   text.append(" NOT NULL");
 }
 return(text);
}

void
addTable(const DBTable& table,DBConnection& connection,bool createColumns)
{
 String tableName(table.getTableName());
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
 String buildTableString;
 if (!tableExists) {
  printf("Creating Table %s\n",tableName.c_str());
  buildTableString.assign("CREATE TABLE ");
  buildTableString.append(databaseName);
  buildTableString.append(".");
  buildTableString.append(tableName);
  buildTableString.append(" (");
 }
 else if (!createColumns)
  return;
 bool first=true;
 DBTable::DBColumnList::const_iterator next=table.getColumnListBegin();
 DBTable::DBColumnList::const_iterator end=table.getColumnListEnd();
 while (next!=end) {
  const DBColumn& column=**next;
  String columnName=column.getColumnName();
  if (!tableExists) {
   printf(" - Column: %s\n",columnName.c_str());
   if (first)
    first=false;
   else
    buildTableString.append(", ");
   buildTableString.append(getColumnDefinitionString(
    column,tableExists));
  } else {
   DBStatement stmt(connection);
   DBResultSet rset(stmt);
   String sqlStatement("SHOW COLUMNS FROM ");
   sqlStatement.append(databaseName);
   sqlStatement.append(".");
   sqlStatement.append(tableName);
   stmt.setSQL(sqlStatement);
   stmt.executeQuery(rset);
   bool found=false;
   while (!found && rset.fetch()) {
    String rowColumnName;
    rset.get(0,rowColumnName);
    if (rowColumnName==columnName)
     found=true;
   }
   if (!found) {
    printf(" - Adding Column %s.%s\n",
     tableName.c_str(),columnName.c_str());
    String sqlStatement("ALTER TABLE ");
    sqlStatement.append(databaseName);
    sqlStatement.append(".");
    sqlStatement.append(tableName);
    sqlStatement.append(" ADD ");
    sqlStatement.append(getColumnDefinitionString(
     column,tableExists));
    DBStatement stmt(connection);
    DBResultSet rset(stmt);
    stmt.setSQL(sqlStatement);
    stmt.executeQuery(rset);
    if (column.getColumnType()==DBColumn::ColumnType::Long)
    {
     SerialColumn& serialColumn=*((SerialColumn*)&column);
     if (serialColumn.getForeignKeyReference())
     {
      String foreignKeyReferenceTableName(
       serialColumn.getForeignKeyReferenceTableName());
      String foreignKeyReferenceColumnName(
       serialColumn.getForeignKeyReferenceColumnName());
      sqlStatement.assign("ALTER TABLE ");
      sqlStatement.append(databaseName);
      sqlStatement.append(".");
      sqlStatement.append(tableName);
      sqlStatement.append(" ADD FOREIGN KEY (");
      sqlStatement.append(columnName);
      sqlStatement.append(") REFERENCES ");
      sqlStatement.append(databaseName);
      sqlStatement.append(".");
      sqlStatement.append(foreignKeyReferenceTableName);
      sqlStatement.append("(");
      sqlStatement.append(foreignKeyReferenceColumnName);
      sqlStatement.append(")");
      stmt.setSQL(sqlStatement);
      stmt.executeQuery(rset);
     }
    }
   }
  }
  ++next;
 }
 if (!tableExists) {
  buildTableString.append(")");
  DBStatement stmt(connection);
  DBResultSet rset(stmt);
  stmt.setSQL(buildTableString);
  stmt.executeQuery(rset);
 }
}

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
  printf("Creating database %s\n",databaseName.c_str());
  DBStatement stmt(connection);
  DBResultSet rset(stmt);
  String statement("CREATE DATABASE ");
  statement.append(databaseName);
  stmt.setSQL(statement);
  stmt.executeQuery(rset);
 }

 std::list<AutoPtr<DBTable>> tableList;
 tableList.emplace_back(new UserTable());
 tableList.emplace_back(new RoleTable());
 tableList.emplace_back(new PermissionTable());
 tableList.emplace_back(new PermissionOverrideTable());

 for (size_t pass=0;pass<2;++pass)
 {
  if (pass==0)
   printf("Creating tables...\n");
  else if (pass==1)
   printf("Modifying tables...\n");

  bool createColumns=(pass>0);
  std::list<AutoPtr<DBTable>>::const_iterator next=tableList.begin();
  std::list<AutoPtr<DBTable>>::const_iterator last=tableList.end();

  while (next!=last)
  {
   bool success=false;
   connection.startTransaction();
   try {
    const DBTable& table=**next;
    addTable(table,connection,createColumns);
    success=true;
   } catch (SysException& excp) {
    printf("ERROR: %s\n",excp.what());
   }
   if (success)
    connection.commit();
   else
    connection.rollback();
   ++next;
  }
 }
 return 0;
}
