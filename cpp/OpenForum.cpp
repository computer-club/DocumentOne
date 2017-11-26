// OpenForum main

#include "Sys.h"
#include "Dbms.h"
#include "QueryProcessor.h"

int
main(int argc, char** argv)
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
     <Serial>1</Serial>\
    </BoardInquiry>\
   </Request>\
  </OpenForumQuery>");
 DBConnection connection;
 connection.connect();
 processor.process(connection);
 printf("%s\n",processor.getResponseString().c_str());
}
