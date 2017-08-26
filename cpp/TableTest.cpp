// main file testing user code

#include <stdio.h>
#include "Sys.h"
#include "User.h"

int
main()
{
 UserTable userTable;
 printf("Table Name: %s\n",userTable.getName().c_str());

 DBTable::DBColumnList::iterator begin=userTable.getColumnListBegin();
 DBTable::DBColumnList::iterator end=userTable.getColumnListEnd();
 while (begin!=end) {
  const DBColumn& column=**begin;
  printf("Column Name: %s\n",column.getColumnName().c_str());
  ++begin;
 }
}
