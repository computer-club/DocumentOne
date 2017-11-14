// column definitions

#ifndef H_COLUMN
#define H_COLUMN

#include "Sys.h"

class DBColumn
{
// public types
public:

 enum class ColumnType
 {
  None,
  String,
  Long,
  Date,
  Time
 };

 enum class IndexType
 {
  None,
  Primary,
  Unique,
  Multiple
 };

// private data
private:
 String columnName;
 String description;
 ColumnType columnType;
 IndexType indexType;

// public functions
public:
 DBColumn()
 { This.columnType=ColumnType::None;
   This.indexType=IndexType::None; }

 void setColumnName(String value)
 { This.columnName=value; }
 const String getColumnName() const
 { return(This.columnName); }

 void setDescription(String value)
 { This.description=value; }
 const String getDescription() const
 { return(This.description); }

 void setColumnType(ColumnType value)
 { This.columnType=value; }
 const ColumnType getColumnType() const
 { return(This.columnType); }

 void setIndexType(IndexType value)
 { This.indexType=value; }
 const IndexType getIndexType() const
 { return(This.indexType); }
};

class StringColumn: public DBColumn
{
// private data
private:
 uint maxLength;

// public functions
public:
 StringColumn()
 { This.setColumnType(ColumnType::String);
   This.setIndexType(IndexType::None); }

 void setMaxLength(uint length)
 { This.maxLength=length; }
 const uint getMaxLength() const
 { return(This.maxLength); }
};

class SerialColumn: public DBColumn
{
// private data
private:
 BoolFalse autoIncrement;
 BoolFalse notNull;
 BoolFalse foreignKeyReference;
 String foreignKeyReferenceTableName;
 String foreignKeyReferenceColumnName;

// public functions
public:
 SerialColumn()
 { This.setColumnType(ColumnType::Long);
   This.setIndexType(IndexType::None); }

 void setAutoIncrement(bool value)
 { This.autoIncrement=value; }
 const bool getAutoIncrement() const
 { return(This.autoIncrement); }
 void setNotNull(bool value)
 { This.notNull=value; }
 const bool getNotNull() const
 { return(This.notNull); }
 void setForeignKeyReference();
 void setForeignKeyReference(
  const String& tableName,
  const String& columnName);
 const bool getForeignKeyReference() const
 { return(This.foreignKeyReference); }
 const String getForeignKeyReferenceTableName() const
 { return(This.foreignKeyReferenceTableName); }
 const String getForeignKeyReferenceColumnName() const
 { return(This.foreignKeyReferenceColumnName); }
};

class DateColumn: public DBColumn
{
// private data
private:
 BoolFalse notNull;

// public functions
public:
 DateColumn()
 { This.setColumnType(ColumnType::Date);
   This.setIndexType(IndexType::None); }

 void setNotNull(bool value)
 { This.notNull=value; }
 const bool getNotNull() const
 { return(This.notNull); }
};

class TimeColumn: public DBColumn
{
// private data
private:
 BoolFalse notNull;

// public functions
public:
 TimeColumn()
 { This.setColumnType(ColumnType::Time);
   This.setIndexType(IndexType::None); }

 void setNotNull(bool value)
 { This.notNull=value; }
 const bool getNotNull() const
 { return(This.notNull); }
};

#endif
