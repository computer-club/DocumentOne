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
const String
 UserTable::tokenColumnName("LOGIN_TOKEN");
const String
 UserTable::tokenExpirationDateColumnName("TOKEN_EXPIRATION_DATE");
const String
 UserTable::tokenExpirationTimeColumnName("TOKEN_EXPIRATION_TIME");

UserTable::UserTable()
{
 This.setTableName(This.getName());
 This.setDescription("User");
 {
  SerialColumn* columnPtr=new SerialColumn();
  SerialColumn& column=*columnPtr;
  column.setColumnName(This.getSerialColumnName());
  column.setDescription("Serial");
  column.setIndexType(SerialColumn::IndexType::Primary);
  column.setAutoIncrement(true);
  column.setNotNull(true);
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
 {
  StringColumn* columnPtr=new StringColumn();
  StringColumn& column=*columnPtr;
  column.setColumnName(This.getTokenColumnName());
  column.setDescription("Login Token");
  column.setMaxLength(User::MaxTokenLength);
  This.addColumn(columnPtr);
 }
 {
  DateColumn* columnPtr=new DateColumn();
  DateColumn& column=*columnPtr;
  column.setColumnName(This.getTokenExpirationDateColumnName());
  column.setDescription("Token Expiration Date");
  This.addColumn(columnPtr);
 }
 {
  TimeColumn* columnPtr=new TimeColumn();
  TimeColumn& column=*columnPtr;
  column.setColumnName(This.getTokenExpirationTimeColumnName());
  column.setDescription("Token Expiration Time");
  This.addColumn(columnPtr);
 }
}
