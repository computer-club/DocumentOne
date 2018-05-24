// permission class definitions

#ifndef H_PERMISSION
#define H_PERMISSION

#include "Sys.h"
#include "Serial.h"
#include "TableBase.h"

class Permission
{
// public types
public:

 class Type;

 class Category
 {
 // public categorys
 public:
  enum Value {
   None,
   ViewBoard,
   ViewTopic,
   ViewPost,
   CreateBoard,
   CreateTopic,
   CreatePost,
   DeleteBoard,
   DeleteTopic,
   DeletePost
  };

  enum {MaxLength=2};
 
 // private data
 private:
  Value value;
 
 // public functions
 public:
  Category()
  { This.value=Category::None; }
  Category(Category::Value right)
  { This.value=right; }
  Category(Category& right)
  { This.value=right.value; }
  Category(const Category& right)
  { This.value=right.value; }
 
  bool fromString(const String& value);
  String toString();
  String getDescription();
  Permission::Type getDefault();
 
  // assignment operators
  Category& operator=(Category::Value right)
  { This.value=right; return(This); }
 
  // comparison operators
  bool operator==(const Category& right) const
  { return(This.value==right.value); }
  bool operator!=(const Category& right) const
  { return(This.value!=right.value); }
 };

 class Type
 {
 // public types
 public:
  enum Value {
   Restricted,
   Allowed,
   AlwaysRestricted,
   AlwaysAllowed
  };

 enum {MaxLength=1};
 
 // private data
 private:
  Value value;
 
 // public functions
 public:
  Type()
  { This.value=Type::Restricted; }
  Type(Type::Value right)
  { This.value=right; }
  Type(Type& right)
  { This.value=right.value; }
  Type(const Type& right)
  { This.value=right.value; }
 
  bool fromString(const String& value);
  String toString();
  String getDescription();
 
  // assignment operators
  Type& operator=(Type::Value right)
  { This.value=right; return(This); }
 
  // comparison operators
  bool operator==(const Type& right) const
  { return(This.value==right.value); }
  bool operator!=(const Type& right) const
  { return(This.value!=right.value); }
 };
};

class PermissionTable: public DBTable
{
// private columns
private:
 static const String name;
 static const String serialColumnName;
 static const String parentSerialColumnName;
 static const String categoryColumnName;
 static const String typeColumnName;

// public functions
public:
 PermissionTable(bool override=false);

 static const String getName()
 { return(PermissionTable::name); }
 static const String getSerialColumnName()
 { return(PermissionTable::serialColumnName); }
 static const String getParentSerialColumnName()
 { return(PermissionTable::parentSerialColumnName); }
 static const String getCategoryColumnName()
 { return(PermissionTable::categoryColumnName); }
 static const String getTypeColumnName()
 { return(PermissionTable::typeColumnName); }
};

class PermissionOverrideTable: public PermissionTable
{
// private columns
private:
 static const String name;

// public functions
public:
 PermissionOverrideTable():
  PermissionTable(true)
 {};

 static const String getName()
 { return(PermissionOverrideTable::name); }
};

#endif
