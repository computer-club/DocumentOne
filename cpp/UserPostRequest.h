// User Post Request class definition

#ifndef H_USER_POST_REQUEST
#define H_USER_POST_REQUEST

#include "Sys.h"
#include "QueryProcessor.h"

class UserPostRequest: public QueryProcessorRequestBase
{
// private data
private:
 DBConnection* connectionPtr;
 String loginToken;
 Serial topicSerial;
 String subject;
 String content;

 Serial postSerial;

// public functions
public:
 UserPostRequest(
  DBConnection& connection,
  QueryRequest& request);

 virtual void perform();
 virtual void generateXML(
  xmlDocPtr document);
};

#endif
