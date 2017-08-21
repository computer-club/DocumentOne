// main file testing user code

#include <stdio.h>
#include "Sys.h"
#include "UserTables.h"

int
main()
{
 UserTable userTable;
 printf("Table Name: %s\n",userTable.getName().c_str());

 DBTable::DBColumnList::iterator begin=userTable.getColumnListBegin();
 DBTable::DBColumnList::iterator end=userTable.getColumnListEnd();
 while (begin!=end) {
  printf("Column Name: %s\n",begin->getColumnName().c_str());
  ++begin;
 }
}
