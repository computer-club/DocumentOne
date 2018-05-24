// OpenForum main

#include "Sys.h"
#include "Dbms.h"
#include "QueryProcessor.h"
#include "SysSocket.h"

void localQueryRequest();
void queryRequestServer(String port);

int
main(int argc, char** argv)
{
 if (argc==1)
 {
  localQueryRequest();
  exit(0);
 }
 else if (argc==2)
 {
  queryRequestServer(argv[1]);
  exit(0);
 }
}

void
queryRequestServer(String port)
{
 Socket server;
 server.listen(port);
 Socket client;
 while (server.accept(client))
 {
  String query;
  client.read(query);

  DBConnection connection;
  connection.connect();
  QueryProcessor processor;
  processor.setQueryString(query);
  processor.process(connection);

  client.write(processor.getResponseString());
//  printf("%s\n",processor.getResponseString().c_str());

  client.disconnect();
 }
}

void
localQueryRequest()
{
 QueryProcessor processor;
 processor.setQueryString("\
  <OpenForumQuery>\
   <Request>\
    <UserPost>\
     <LoginToken></LoginToken>\
     <TopicSerial>1</TopicSerial>\
     <Subject>TESTING</Subject>\
     <Content>Working Password!</Content>\
    </UserPost>\
   </Request>\
  </OpenForumQuery>");
 processor.setQueryString("\
  <OpenForumQuery>\
   <Request>\
    <UserLoginVerify>\
     <Username></Username>\
     <Password></Password>\
    </UserLoginVerify>\
   </Request>\
  </OpenForumQuery>");
 processor.setQueryString("\
  <OpenForumQuery>\
   <Request>\
    <TopicInquiry>\
     <Serial>1</Serial>\
    </TopicInquiry>\
   </Request>\
  </OpenForumQuery>");
 processor.setQueryString("\
  <OpenForumQuery>\
   <Request>\
    <UserLogout>\
     <LoginToken></LoginToken>\
    </UserLogout>\
   </Request>\
  </OpenForumQuery>");
 processor.setQueryString("\
  <OpenForumQuery>\
   <Request>\
    <UserRegister>\
     <Username></Username>\
     <Password></Password>\
    </UserRegister>\
   </Request>\
  </OpenForumQuery>");
 processor.setQueryString("\
  <OpenForumQuery>\
   <Request>\
    <ForumInquiry />\
   </Request>\
  </OpenForumQuery>");
 processor.setQueryString("\
  <OpenForumQuery>\
   <Request>\
    <BoardInquiry>\
     <Serial>3</Serial>\
    </BoardInquiry>\
   </Request>\
  </OpenForumQuery>");
/*
 processor.setQueryString("\
  <OpenForumQuery>\
   <Request>\
    <UserInquiry>\
     <Serial>1</Serial>\
    </UserInquiry>\
   </Request>\
  </OpenForumQuery>");
*/
 DBConnection connection;
 connection.connect();
 processor.process(connection);
 printf("%s\n",processor.getResponseString().c_str());
}
