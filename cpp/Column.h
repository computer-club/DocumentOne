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
  Long
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
 BoolTrue autoIncrement;
 BoolTrue notNull;

// public functions
public:
 SerialColumn()
 { This.setColumnType(ColumnType::Long);
   This.setIndexType(IndexType::Primary); }

 void setAutoIncrement(bool value)
 { This.autoIncrement=value; }
 const bool getAutoIncrement() const
 { return(This.autoIncrement); }
 void setNotNull(bool value)
 { This.notNull=value; }
 const bool getNotNull() const
 { return(This.notNull); }
};

#endif
