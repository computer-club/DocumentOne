// SQL Test Code

#include <stdio.h>
#include "Sys.h"
#include "Dbms.h"

int
main()
{
 DBConnection connection;
 connection.connect();

 try
 {
  DBStatement stmt(connection);
  DBResultSet rset(stmt);
  stmt.setSQL("SHOW FAIL");
  stmt.executeQuery(rset);
 } catch (SysException& excp) {
  printf("# %s\n",excp.what());
 }

 {
  DBStatement stmt(connection);
  DBResultSet rset(stmt);
  stmt.setSQL("SHOW CREATE TABLE CORE.USER");
  stmt.executeQuery(rset);
  while (rset.fetch())
  {
   String tableName;
   String creationCode;
   rset.get(0,tableName);
   rset.get(1,creationCode);
   printf("> %s\n",tableName.c_str());
   printf("> %s\n",creationCode.c_str());
   printf("\n\n");
  }
 }
 printf("Finishing\n");
}
