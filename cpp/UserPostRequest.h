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

class UserTopicPostRequest: public QueryProcessorRequestBase
{
// private data
private:
 DBConnection* connectionPtr;
 String loginToken;
 Serial boardSerial;
 String subject;
 String content;

 Serial topicSerial;
 Serial postSerial;

// public functions
public:
 UserTopicPostRequest(
  DBConnection& connection,
  QueryRequest& request);

 virtual void perform();
 virtual void generateXML(
  xmlDocPtr document);
};

#endif
