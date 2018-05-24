// Database Management System definitions

// Currently, the plan is to write direct support to MariaSQL and
// potentially expand from there. Everything will be written generic
// but no other DBMS are expected to work inherently.

#include <mysql.h>
#include <my_global.h>
#include "Sys.h"
#include "SysException.h"
#include "Serial.h"

#define DBLOGMODE 0

class DBStatement;
class DBResultSet;

class DBConnection
{
 friend class DBStatement;
 friend class DBResultSet;

// public types
public:
 typedef String Username;
 typedef String Password;
 typedef String Schema;
 typedef String Host;
 typedef String DBIdentifier;
 typedef unsigned int Port;
 typedef unsigned long ClientFlag;

 class RawConnection
 {
  friend class DBConnection;
 
 // private data
 private:
  MYSQL* value;

 // public functions
 public:
  RawConnection()
  { This.value=NULL; }
  RawConnection(MYSQL* ptr)
  { This.value=ptr; }
  ~RawConnection()
  { This.close(); }

  MYSQL* getValue()
  { return(This.value); }

 // private functions
 private:
  void close();

  bool setValue(MYSQL* ptr);

  MYSQL& operator*()
  { return(*This.value); }
  MYSQL* operator->()
  { return(This.value); }
 };

// private data
private:
 RawConnection connectionPtr;
 Username username;
 Password password;
 Schema defaultSchema;
 Host host;
 Port port;
 DBIdentifier databaseName;
 ClientFlag clientFlag;

// public functions
public:
 DBConnection();

 void setUsername(const Username& value)
 { This.username=value; }
 const Username& getUsername() const
 { return(This.username); }
 void setPassword(const Password& value)
 { This.password=value; }
 const Password& getPassword() const
 { return(This.password); }
 void setDefaultSchema(const Schema& value)
 { This.defaultSchema=value; }
 const Schema& getSchema() const
 { return(This.defaultSchema); }
 void setHost(const Host& value)
 { This.host=value; }
 const Host& getHost() const
 { return(This.host); }
 void setPort(const Port value)
 { This.port=value; }
 const Port getPort() const
 { return(This.port); }
 void setDatabaseName(const DBIdentifier& value)
 { This.databaseName=value; }
 const DBIdentifier& getDatabaseName() const
 { return(This.databaseName); }

 void connect();
 void close();
 void startTransaction();
 void commit();
 void rollback();
};

class DBStatement
{
 friend class DBResultSet;

// private types
private:
 typedef String DBIdentifier;
 typedef String Statement;

// private data
private:
 DBConnection* connection;
 Statement stmt;

// public functions
public:
 DBStatement(DBConnection& connection)
 { This.connection=&connection; }

 void clear()
 { This.stmt.clear(); }

 void setSelect()
 { This.stmt.assign("SELECT "); }
 void addSelect()
 { This.stmt.append("SELECT "); }
 void setInsertInto()
 { This.stmt.assign("INSERT INTO "); }
 void setUpdate()
 { This.stmt.assign("UPDATE "); }

 void addComma()
 { This.stmt.append(","); }
 void addLeftParenthesis()
 { This.stmt.append("("); }
 void addRightParenthesis()
 { This.stmt.append(")"); }

 void addAnd()
 { This.stmt.append(" AND "); }
 void addValues()
 { This.stmt.append(" VALUES "); }
 void addSet()
 { This.stmt.append(" SET "); }
 void addFrom()
 { This.stmt.append(" FROM "); }
 void addJoin()
 { This.stmt.append(" JOIN "); }
 void addOn()
 { This.stmt.append(" ON "); }
 void addWhere()
 { This.stmt.append(" WHERE "); }
 void addOrderBy()
 { This.stmt.append(" ORDER BY "); }
 void addGroupBy()
 { This.stmt.append(" GROUP BY "); }

 void addIsEqualTo()
 { This.stmt.append("="); }
 void addIsNotEqualTo()
 { This.stmt.append("!="); }
 void addIsGreaterThan()
 { This.stmt.append(">"); }
 void addIsGreaterThanOrEqualTo()
 { This.stmt.append(">="); }
 void addIsLessThan()
 { This.stmt.append("<"); }
 void addIsLessThanOrEqualTo()
 { This.stmt.append("<="); }

 void addIdentifier(const DBIdentifier& columnName)
 { This.stmt.append(columnName); }
 void addIdentifier(
  const DBIdentifier& tableName,
  const DBIdentifier& columnName);

 void addLiteral(const String& value);
 void addLiteral(const Serial& value);

 void addLastInsertId()
 { This.stmt.append("LAST_INSERT_ID()"); }

 const Statement& getSQL() const
 { return(This.stmt); }
 void setSQL(const Statement& value)
 { This.stmt=value; }

 void executeQuery(DBResultSet& rset);
};

class DBResultSet
{
 friend class DBStatement;

// private types
private:
 typedef String DBIdentifier;

// private data
private:
 MYSQL_RES* rsetPtr;
 MYSQL_ROW currRow;
 DBStatement* stmtPtr;
 size_t columnCount;
 
// public functions
public:
 DBResultSet(DBStatement& stmt);
 ~DBResultSet();

 bool fetch();

 void get(size_t pos,String& value);
 void get(size_t post,Serial& value);

// private functions
 void storeResult(DBConnection* connection);
 void storeResult(MYSQL_STMT* statement);
};

class DBException: public SysException
{
// public functions
public:
 DBException(String text)
 { This.setExceptionText(text); }
};
