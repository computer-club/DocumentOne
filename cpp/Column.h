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
  String
 };

// private data
private:
 String columnName;
 String description;
 ColumnType columnType;

// public functions
public:
 DBColumn()
 { This.columnType=ColumnType::None; }

 void setColumnName(String value)
 { This.columnName=value; }
 String getColumnName()
 { return(This.columnName); }

 void setDescription(String value)
 { This.description=value; }
 String getDescription()
 { return(This.description); }

 void setColumnType(ColumnType value)
 { This.columnType=value; }
 const ColumnType getColumnType() const
 { return(This.columnType); }
};

class StringColumn: public DBColumn
{
// private data
private:
 uint maxLength;

// public functions
public:
 StringColumn()
 { This.setColumnType(ColumnType::String); }

 void setMaxLength(uint length)
 { This.maxLength=length; }
 uint getMaxLength()
 { return(This.maxLength); }
};

#endif
