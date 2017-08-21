// Table base class

#ifndef H_TABLE_BASE
#define H_TABLE_BASE

#include "Sys.h"
#include "Column.h"
#include <list>

class DBTable
{
// public types
public:
 typedef std::list<DBColumn> DBColumnList;

// private data
private:
 String tableName;
 String description;
 DBColumnList columnList;

// public functions
public:
 void setTableName(const String& tableName)
 { This.tableName.assign(tableName); }
 const String getTableName() const
 { return(This.tableName); }
 void setDescription(const String& description)
 { This.description.assign(description); }
 const String& getDescription() const
 { return(This.description); }
 void addColumn(const DBColumn& column)
 { columnList.push_back(column); }

 DBColumnList::iterator getColumnListBegin()
 { return(This.columnList.begin()); }
 DBColumnList::iterator getColumnListEnd()
 { return(This.columnList.end()); }
};

#endif
