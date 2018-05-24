// Post Inquiry Request class definitions

#ifndef H_POST_INQUIRY_REQUEST
#define H_POST_INQUIRY_REQUEST

#include "Sys.h"
#include "QueryProcessor.h"
#include "Serial.h"
#include "PostData.h"
#include "UserData.h"

class ForumInquiryRequest: public QueryProcessorRequestBase
{
// public types
public:
 typedef std::list<Serial> SerialList;

// private data
private:
 DBConnection* connectionPtr;
 SerialList boardSerialList;

// public functions
public:
 ForumInquiryRequest(
  DBConnection& connection,
  QueryRequest& request);

 virtual void perform();
 virtual void generateXML(
  xmlDocPtr document);

 SerialList::const_iterator boardSerialListBegin()
 { return(This.boardSerialList.begin()); }
 SerialList::const_iterator boardSerialListEnd()
 { return(This.boardSerialList.end()); }
};

class BoardInquiryRequest: public QueryProcessorRequestBase
{
// private data
private:
 DBConnection* connectionPtr;
 Serial boardSerial;
 BoardData boardData;

// public functions
public:
 BoardInquiryRequest(
  DBConnection& connection,
  QueryRequest& request);

 virtual void perform();
 virtual void generateXML(
  xmlDocPtr document);
};

class TopicInquiryRequest: public QueryProcessorRequestBase
{
// private data
private:
 DBConnection* connectionPtr;
 Serial topicSerial;
 TopicData topicData;

// public functions
public:
 TopicInquiryRequest(
  DBConnection& connection,
  QueryRequest& request);

 virtual void perform();
 virtual void generateXML(
  xmlDocPtr document);
};

class PostInquiryRequest: public QueryProcessorRequestBase
{
// private data
private:
 DBConnection* connectionPtr;
 Serial postSerial;
 PostData postData;

// public functions
public:
 PostInquiryRequest(
  DBConnection& connection,
  QueryRequest& request);

 virtual void perform();
 virtual void generateXML(
  xmlDocPtr document);
};

class UserInquiryRequest: public QueryProcessorRequestBase
{
// private data
private:
 DBConnection* connectionPtr;
 Serial userSerial;
 UserData userData;

// public functions
public:
 UserInquiryRequest(
  DBConnection& connection,
  QueryRequest& request);

 virtual void perform();
 virtual void generateXML(
  xmlDocPtr document);
};

#endif
