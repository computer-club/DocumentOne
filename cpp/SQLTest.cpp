// SQL Test Code

#include <stdio.h>
#include "Sys.h"
#include "Dbms.h"

int
main()
{
 DBConnection connection;
 connection.connect();

 DBStatement stmt(connection);
 DBResultSet rset(stmt);
 stmt.setSQL("SHOW DATABASES");
 stmt.executeQuery(rset);
 while (rset.fetch()) {
  String databaseName;
  rset.get(0,databaseName);
  printf("%s\n",databaseName.c_str());
 }
 return 0;
}
