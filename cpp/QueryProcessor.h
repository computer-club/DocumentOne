// Query Processor class definition

#ifndef H_QUERY_PROCESSOR
#define H_QUERY_PROCESSOR

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <list>
#include "Sys.h"

class DBConnection;
class QueryRequest;

class QueryProcessor
{
// public types
public:
 typedef String XMLString;

// private data
private:
 XMLString queryString;
 XMLString responseString;
 xmlDocPtr queryDocument;
 xmlNode* lastNode;

// public functions
public:
 QueryProcessor();

 void process(DBConnection& connection);

 void setQueryString(const XMLString& value)
 { This.queryString=value; }
 const XMLString& getQueryString() const
 { return(This.queryString); }
 const XMLString& getResponseString() const
 { return(This.responseString); }

// private functions
private:
 void validateRootElement();
 bool getNextRequest(QueryRequest& request);
};

class QueryProcessorRequestBase
{
// public functions
public:
 QueryProcessorRequestBase()
 {}

 virtual void perform()=0;
 virtual void generateXML(xmlDocPtr document)=0;
};

class QueryRequest
{
// public types
public:

 enum class RequestType
 {
  None,
  ForumInquiry,
  BoardInquiry,
  TopicInquiry,
  PostInquiry,
  UserInquiry,
  UserLoginVerify,
  UserRegister,
  UserLogout,
  UserPermission,
  UserPost,
  UserTopicPost
 };

 class Element
 {
 // public types
 public:

  class Attribute
  {
  // private data
  private:
   String name;
   String value;

  // public data
  public:
   const String& getName() const
   { return(This.name); }
   void setName(const String& value)
   { This.name=value; }
   const String& getValue() const
   { return(This.value); }
   void setValue(const String& value)
   { This.value=value; }
  };

  typedef std::list<Attribute> AttributeList;

 // private data
 private:
  String name;
  AttributeList attributeList;
  String value;

 // public functions
 public:
  const String& getName() const
  { return(This.name); }
  void setName(const String& value)
  { This.name=value; }
  const String& getValue() const
  { return(This.value); }
  void setValue(const String& value)
  { This.value=value; }

  Attribute& pushAttribute()
  { This.attributeList.emplace_back();
    return(This.attributeList.back()); }

  AttributeList::const_iterator beginAttributeList() const
  { return(This.attributeList.begin()); }
  AttributeList::const_iterator endAttributeList() const
  { return(This.attributeList.end()); }

  bool fromXML(xmlNode* requestData);
 };

 typedef std::list<Element> ElementList;

// private data
private:
 RequestType type;
 ElementList elementList;
 AutoPtr<QueryProcessorRequestBase> processorRequest;

// public functions
public:
 QueryRequest();

 ElementList::const_iterator elementListBegin()
 { return(This.elementList.begin()); }
 ElementList::const_iterator elementListEnd()
 { return(This.elementList.end()); }

 bool fromXML(xmlNode* requestData);
 void perform(DBConnection& connection);
 void generateXML(xmlDocPtr responseDoc);
 void clear();

 RequestType requestTypeFromString(String& value) const;
};

#endif
