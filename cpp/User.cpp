// user table code

#include "Sys.h"
#include "User.h"

const String
 UserTable::name("USER");
const String
 UserTable::serialColumnName("SERIAL");
const String
 UserTable::usernameColumnName("USERNAME");
const String
 UserTable::passwordColumnName("PASSWORD");

UserTable::UserTable()
{
 This.setTableName(This.getName());
 This.setDescription("User");
 {
  SerialColumn* columnPtr=new SerialColumn();
  SerialColumn& column=*columnPtr;
  column.setColumnName(This.getSerialColumnName());
  column.setDescription("Serial");
  This.addColumn(columnPtr);
 }
 {
  StringColumn* columnPtr=new StringColumn();
  StringColumn& column=*columnPtr;
  column.setColumnName(This.getUsernameColumnName());
  column.setDescription("Username");
  column.setMaxLength(User::MaxUsernameLength);
  This.addColumn(columnPtr);
 }
 {
  StringColumn* columnPtr=new StringColumn();
  StringColumn& column=*columnPtr;
  column.setColumnName(This.getPasswordColumnName());
  column.setDescription("Password");
  column.setMaxLength(User::MaxPasswordLength);
  This.addColumn(columnPtr);
 }
}
