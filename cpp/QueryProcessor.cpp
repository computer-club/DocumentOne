// Query Processor code definition

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "Sys.h"
#include "QueryProcessor.h"
#include "Dbms.h"
#include "PostInquiryRequest.h"
#include "UserPostRequest.h"
#include "UserControlRequest.h"

QueryProcessor::QueryProcessor()
{
 This.queryDocument=NULL;
 This.lastNode=NULL;
}

void
QueryProcessor::process(DBConnection& connection)
{
 xmlDocPtr responseDoc=xmlNewDoc(BAD_CAST "1.0");
 xmlNodePtr responseRootNode=xmlNewNode(NULL, BAD_CAST "OpenForumQuery");
 xmlDocSetRootElement(responseDoc,responseRootNode);
 if (This.queryString.empty())
 {
  xmlNewChild(responseRootNode,NULL,
   BAD_CAST "SysException",
   BAD_CAST "No Query");
 }
 else
 {
  xmlDocPtr inputDoc=xmlReadMemory(
   This.queryString.c_str(),
   This.queryString.size(),
   "",NULL,0);
  try {
   if (inputDoc==NULL)
    throw SysException("Query Parse Failed");
   This.queryDocument=inputDoc;
   This.validateRootElement();

   QueryRequest request;
   while (This.getNextRequest(request)) {
    request.perform(connection);
    request.generateXML(responseDoc);
    request.clear();
   }

   connection.commit();
   // connection.rollback();
  }
  catch(const SysException& excp)
  {
   xmlFreeDoc(inputDoc);
   xmlNewChild(responseRootNode,NULL,
    BAD_CAST "SysException",
    BAD_CAST excp.what());
  }
  catch(...)
  {
   xmlFreeDoc(inputDoc);
   xmlNewChild(responseRootNode,NULL,
    BAD_CAST "SysException",
    BAD_CAST "Unknown Exception");
  }
 }

 xmlChar* responseText;
 int responseSize=0;
 try {
  xmlDocDumpFormatMemory(responseDoc,&responseText,&responseSize,1);
  This.responseString.assign((char*)responseText,responseSize);
 }
 catch(...)
 {
  xmlFree(responseText);
  xmlFreeDoc(responseDoc);
  throw;
 }
 xmlFree(responseText);
 xmlFreeDoc(responseDoc);
}

void
QueryProcessor::validateRootElement()
{
 xmlNode* rootElement=xmlDocGetRootElement(This.queryDocument);

 if (rootElement->type!=XML_ELEMENT_NODE)
  throw SysException("Query Parse Failed");

 String rootElementName((char*)rootElement->name);
 if (rootElementName!="OpenForumQuery")
  throw SysException("Query does not start with OpenForumQuery");
}

bool
QueryProcessor::getNextRequest(QueryRequest& request)
{
 bool valid=false;
 if (This.queryDocument==NULL)
  throw SysException("No query found");
 if (This.lastNode==NULL)
 {
  xmlNode* rootElement=xmlDocGetRootElement(This.queryDocument);
  This.lastNode=rootElement->children;
 }
 else
  This.lastNode=This.lastNode->next;

 while (This.lastNode && This.lastNode->type!=XML_ELEMENT_NODE)
  This.lastNode=This.lastNode->next;

 if (This.lastNode)
 {
  String nodeName((char*)This.lastNode->name);
  if (nodeName!="Request")
  {
   String text("Invalid node found: ");
   text.append(nodeName);
   throw SysException(text);
  }
  if (This.lastNode->children)
   valid=true;
 }
 if (valid)
 {
  xmlNode* childNode=This.lastNode->children;
  while (childNode && childNode->type!=XML_ELEMENT_NODE)
   childNode=childNode->next;
  if (!request.fromXML(childNode))
   valid=false;
 }
 return(valid);
}

QueryRequest::QueryRequest()
{
 This.type=RequestType::None;
 This.elementList.clear();
}

bool
QueryRequest::fromXML(xmlNode* requestData)
{
 bool valid=true;
 if (requestData->type!=XML_ELEMENT_NODE)
  throw SysException("Invalid node type");

 String nodeName((char*)requestData->name);
 This.type=This.requestTypeFromString(nodeName);

 xmlNode* child=requestData->children;
 while (child)
 {
  if (child->type==XML_ELEMENT_NODE)
  {
   This.elementList.emplace_back();
   Element& childElem=This.elementList.back();
   childElem.fromXML(child);
  }
  child=child->next;
 }
 return(valid);
}

void
QueryRequest::perform(DBConnection& connection)
{
 if (This.type==RequestType::None)
  throw SysException("Unable to perform request type 'None'");
 else if (This.type==RequestType::ForumInquiry)
 {
  ForumInquiryRequest* requestPtr=
   new ForumInquiryRequest(connection,This);
  This.processorRequest.reset(requestPtr);
  requestPtr->perform();
 }
 else if (This.type==RequestType::BoardInquiry)
 {
  BoardInquiryRequest* requestPtr=
   new BoardInquiryRequest(connection,This);
  This.processorRequest.reset(requestPtr);
  requestPtr->perform();
 }
 else if (This.type==RequestType::TopicInquiry)
 {
  TopicInquiryRequest* requestPtr=
   new TopicInquiryRequest(connection,This);
  This.processorRequest.reset(requestPtr);
  requestPtr->perform();
 }
 else if (This.type==RequestType::PostInquiry)
 {
  PostInquiryRequest* requestPtr=
   new PostInquiryRequest(connection,This);
  This.processorRequest.reset(requestPtr);
  requestPtr->perform();
 }
 else if (This.type==RequestType::UserPost)
 {
  UserPostRequest* requestPtr=
   new UserPostRequest(connection,This);
  This.processorRequest.reset(requestPtr);
  requestPtr->perform();
 }
 else if (This.type==RequestType::UserLoginVerify)
 {
  UserLoginRequest* requestPtr=
   new UserLoginRequest(connection,This);
  This.processorRequest.reset(requestPtr);
  // perform not necessary
 }
 else if (This.type==RequestType::UserRegister)
 {
  UserRegisterRequest* requestPtr=
   new UserRegisterRequest(connection,This);
  This.processorRequest.reset(requestPtr);
  // perform not necessary
 }
 else if (This.type==RequestType::UserLogout)
 {
  UserLogoutRequest* requestPtr=
   new UserLogoutRequest(connection,This);
  This.processorRequest.reset(requestPtr);
  requestPtr->perform();
 }
 else
  throw SysException("Request type not coded for");
}

void
QueryRequest::generateXML(xmlDocPtr responseDoc)
{
 This.processorRequest->generateXML(responseDoc);
}

void
QueryRequest::clear()
{
 This.type=RequestType::None;
 This.elementList.clear();
}

QueryRequest::RequestType
QueryRequest::requestTypeFromString(String& value) const
{
 RequestType result;
 if (value=="None")
  result=RequestType::None;
 else if (value=="ForumInquiry")
  result=RequestType::ForumInquiry;
 else if (value=="BoardInquiry")
  result=RequestType::BoardInquiry;
 else if (value=="TopicInquiry")
  result=RequestType::TopicInquiry;
 else if (value=="PostInquiry")
  result=RequestType::PostInquiry;
 else if (value=="UserLoginVerify")
  result=RequestType::UserLoginVerify;
 else if (value=="UserRegister")
  result=RequestType::UserRegister;
 else if (value=="UserLogout")
  result=RequestType::UserLogout;
 else if (value=="UserPost")
  result=RequestType::UserPost;
 else
 {
  String text("Invalid option string '");
  text.append(value);
  text.append("'");
  throw SysException(text);
 }
 return(result);
}

bool
QueryRequest::Element::fromXML(xmlNode* requestData)
{
 bool valid=true;
 if (requestData->type!=XML_ELEMENT_NODE)
  throw SysException("Invalid node type");

 String nodeName((char*)requestData->name);
 This.setName(nodeName);

 if (requestData->children)
 {
  xmlNode* child=requestData->children;
  String value;
  if (child->type==XML_TEXT_NODE)
   value.assign((char*)child->content);
  This.setValue(value);
 }

 if (requestData->properties)
 {
  xmlAttr* property=requestData->properties;
  String propertyName((char*)property->name);
  String propertyValue;
  if (property->children)
  {
   xmlNode* child=property->children;
   if (child->type==XML_TEXT_NODE)
    propertyValue.assign((char*)child->content);
  }
  Attribute& attribute=This.pushAttribute();
  attribute.setName(propertyName);
  attribute.setValue(propertyValue);
 }
 return(valid);
}
