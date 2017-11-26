// OpenForum main

#include "Sys.h"
#include "Dbms.h"
#include "PostData.h"

int
main(int argc, char** argv)
{
 DBConnection connection;
 connection.connect();
 TopicData topicData;
 topicData.retrieve(connection,1);
 printf("Board:   %s\nUser:    %s\nSubject: %s\n",
  topicData.getBoardSerial().toString().c_str(),
  topicData.getUserSerial().toString().c_str(),
  topicData.getSubject().c_str());
 TopicData::SerialList::const_iterator next(
  topicData.beginPostSerialList());
 TopicData::SerialList::const_iterator last(
  topicData.endPostSerialList());
 while (next!=last)
 {
  printf("Retrieved Post:\n");
  PostData postData;
  postData.retrieve(connection,*next);
  printf("Topic:   %s\nUser:    %s\nSubject: %s\nContent: %s\n",
   postData.getTopicSerial().toString().c_str(),
   postData.getUserSerial().toString().c_str(),
   postData.getSubject().c_str(),
   postData.getContent().c_str());
  ++next;
 }
}
