// column code definitions

#include "Sys.h"
#include "Column.h"

void
SerialColumn::setForeignKeyReference()
{
 This.foreignKeyReference=false;
 This.foreignKeyReferenceTableName.clear();
 This.foreignKeyReferenceColumnName.clear();
}

void
SerialColumn::setForeignKeyReference(
 const String& tableName,
 const String& columnName)
{
 This.foreignKeyReference=true;
 This.foreignKeyReferenceTableName=tableName;
 This.foreignKeyReferenceColumnName=columnName;
}
