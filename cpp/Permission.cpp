#include <stdio.h>
#include "Sys.h"
#include "SysException.h"
#include "Permission.h"
#include "User.h"

const String
 PermissionTable::name("PERMISSION");
const String
 PermissionTable::serialColumnName("SERIAL");
const String
 PermissionTable::parentSerialColumnName("PARENT_SERIAL");
const String
 PermissionTable::categoryColumnName("CATEGORY");
const String
 PermissionTable::typeColumnName("TYPE");

const String
 PermissionOverrideTable::name("PERMISSION_OVERRIDE");

bool
Permission::Category::fromString(const String& value)
{
 if (value=="NO")
  This.value=None;
 else if (value=="VB")
  This.value=ViewBoard;
 else if (value=="VT")
  This.value=ViewTopic;
 else if (value=="VP")
  This.value=ViewPost;
 else if (value=="CB")
  This.value=CreateBoard;
 else if (value=="CT")
  This.value=CreateTopic;
 else if (value=="CP")
  This.value=CreatePost;
 else if (value=="DB")
  This.value=DeleteBoard;
 else if (value=="DT")
  This.value=DeleteTopic;
 else if (value=="DP")
  This.value=DeletePost;
 else
  return(false);
 return(true);
}

String
Permission::Category::toString()
{
 String result;
 if (This.value==None)
  result="NO";
 else if (This.value==ViewBoard)
  result="VB";
 else if (This.value==ViewTopic)
  result="VT";
 else if (This.value==ViewPost)
  result="VP";
 else if (This.value==CreateBoard)
  result="CB";
 else if (This.value==CreateTopic)
  result="CT";
 else if (This.value==CreatePost)
  result="CP";
 else if (This.value==DeleteBoard)
  result="DB";
 else if (This.value==DeleteTopic)
  result="DT";
 else if (This.value==DeletePost)
  result="DP";
 else
 {
  String exceptionText("No string entry for value");
  throw SysException(exceptionText);
 }
 return(result);
}

String
Permission::Category::getDescription()
{
 String result;
 if (This.value==None)
  result="None";
 else if (This.value==ViewBoard)
  result="View Board";
 else if (This.value==ViewTopic)
  result="View Topic";
 else if (This.value==ViewPost)
  result="View Post";
 else if (This.value==CreateBoard)
  result="Create Board";
 else if (This.value==CreateTopic)
  result="Create Topic";
 else if (This.value==CreatePost)
  result="Create Post";
 else if (This.value==DeleteBoard)
  result="Delete Board";
 else if (This.value==DeleteTopic)
  result="Delete Topic";
 else if (This.value==DeletePost)
  result="Delete Post";
 else
 {
  String exceptionText("No description entry for value");
  throw SysException(exceptionText);
 }
 return(result);
}

Permission::Type
Permission::Category::getDefault()
{
 Permission::Type result;
 if (This.value==None)
  result=Permission::Type::Allowed;
 else if (This.value==ViewBoard)
  result=Permission::Type::Allowed;
 else if (This.value==ViewTopic)
  result=Permission::Type::Allowed;
 else if (This.value==ViewPost)
  result=Permission::Type::Allowed;
 else if (This.value==CreateBoard)
  result=Permission::Type::Restricted;
 else if (This.value==CreateTopic)
  result=Permission::Type::Allowed;
 else if (This.value==CreatePost)
  result=Permission::Type::Allowed;
 else if (This.value==DeleteBoard)
  result=Permission::Type::Restricted;
 else if (This.value==DeleteTopic)
  result=Permission::Type::Restricted;
 else if (This.value==DeletePost)
  result=Permission::Type::Restricted;
 else
 {
  String exceptionText("No default entry for value");
  throw SysException(exceptionText);
 }
 return(result);
}

bool
Permission::Type::fromString(const String& value)
{
 if (value=="r")
  This.value=Restricted;
 else if (value=="a")
  This.value=Allowed;
 else if (value=="R")
  This.value=AlwaysRestricted;
 else if (value=="A")
  This.value=AlwaysAllowed;
 else
  return(false);
 return(true);
}

String
Permission::Type::toString()
{
 String result;
 if (This.value==Restricted)
  result="r";
 else if (This.value==Allowed)
  result="a";
 else if (This.value==AlwaysRestricted)
  result="R";
 else if (This.value==AlwaysAllowed)
  result="A";
 else
 {
  String exceptionText("No string entry for value");
  throw SysException(exceptionText);
 }
 return(result);
}

String
Permission::Type::getDescription()
{
 String result;
 if (This.value==Restricted)
  result="Restricted";
 else if (This.value==Allowed)
  result="Allowed";
 else if (This.value==AlwaysRestricted)
  result="Always Restricted";
 else if (This.value==AlwaysAllowed)
  result="Always Allowed";
 else
 {
  String exceptionText("No description entry for value");
  throw SysException(exceptionText);
 }
 return(result);
}

PermissionTable::PermissionTable(bool override)
{
 if (!override)
 {
  This.setTableName(PermissionTable::getName());
  This.setDescription("Permission");
 }
 else
 {
  This.setTableName(PermissionOverrideTable::getName());
  This.setDescription("Permission Override");
 }
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
  SerialColumn* columnPtr=new SerialColumn();
  SerialColumn& column=*columnPtr;
  column.setColumnName(This.getParentSerialColumnName());
  column.setDescription("Parent Serial");
  column.setIndexType(SerialColumn::IndexType::Multiple);
  if (!override)
   column.setForeignKeyReference(
    RoleTable::getName(),
    RoleTable::getSerialColumnName());
  else
   column.setForeignKeyReference(
    UserTable::getName(),
    UserTable::getSerialColumnName());
  column.setNotNull(true);
  This.addColumn(columnPtr);
 }
 {
  StringColumn* columnPtr=new StringColumn();
  StringColumn& column=*columnPtr;
  column.setColumnName(This.getCategoryColumnName());
  column.setDescription("Category");
  column.setMaxLength(Permission::Category::MaxLength);
  This.addColumn(columnPtr);
 }
 {
  StringColumn* columnPtr=new StringColumn();
  StringColumn& column=*columnPtr;
  column.setColumnName(This.getTypeColumnName());
  column.setDescription("Type");
  column.setMaxLength(Permission::Type::MaxLength);
  This.addColumn(columnPtr);
 }
}
