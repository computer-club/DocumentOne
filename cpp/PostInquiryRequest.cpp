// Post Inquiry Request classes code

#include <libxml/tree.h>
#include "Sys.h"
#include "SysException.h"
#include "Dbms.h"
#include "PostInquiryRequest.h"

ForumInquiryRequest::ForumInquiryRequest(
 DBConnection& connection,
 QueryRequest& request)
{
 This.connectionPtr=&connection;
 QueryRequest::ElementList::const_iterator next(
  request.elementListBegin());
 QueryRequest::ElementList::const_iterator last(
  request.elementListEnd());
 if (next!=last)
 {
  String elementName(next->getName());
  String elementValue(next->getValue());
  String text("Invalid request element '");
  text.append(elementName);
  text.append("'");
  throw SysException(text);
 }
}

void
ForumInquiryRequest::perform()
{
 DBConnection& connection=*This.connectionPtr;
 DBStatement stmt(connection);
 DBResultSet rset(stmt);
 stmt.setSelect();
 stmt.addIdentifier(BoardTable::getSerialColumnName());
 stmt.addFrom();
 stmt.addIdentifier(BoardTable::getName());
 stmt.executeQuery(rset);
 while (rset.fetch())
 {
  Serial boardSerial;
  rset.get(0,boardSerial);
  This.boardSerialList.push_back(boardSerial);
 }
}

void
ForumInquiryRequest::generateXML(xmlDocPtr document)
{
 DBConnection& connection=*This.connectionPtr;

 xmlNode* rootElement=xmlDocGetRootElement(document);
 xmlNode* forumElement=xmlNewChild(rootElement,NULL,
  BAD_CAST "ForumInquiry",NULL);
 xmlNode* boardsElement=xmlNewChild(forumElement,NULL,
  BAD_CAST "Boards",NULL);

 BoardData::SerialList::const_iterator next(
  This.boardSerialListBegin());
 BoardData::SerialList::const_iterator last(
  This.boardSerialListEnd());
 while (next!=last)
 {
  const Serial& serial=*next;
  BoardData boardData;
  boardData.retrieve(connection,serial);
  String serialText(boardData.getSerial().toString());
  String description(boardData.getDescription());

  xmlNode* boardElement=xmlNewChild(boardsElement,NULL,
   BAD_CAST "Board",NULL);

  xmlNewChild(boardElement,NULL,
   BAD_CAST "Serial",
   BAD_CAST serialText.c_str());
  xmlNewChild(boardElement,NULL,
   BAD_CAST "Description",
   BAD_CAST description.c_str());
  ++next;
 }
}

BoardInquiryRequest::BoardInquiryRequest(
 DBConnection& connection,
 QueryRequest& request)
{
 This.connectionPtr=&connection;
 QueryRequest::ElementList::const_iterator next(
  request.elementListBegin());
 QueryRequest::ElementList::const_iterator last(
  request.elementListEnd());
 while (next!=last)
 {
  String elementName(next->getName());
  String elementValue(next->getValue());
  if (elementName!="Serial")
  {
   String text("Invalid request element '");
   text.append(elementName);
   text.append("'");
   throw SysException(text);
  }
  else
   This.boardSerial.fromString(elementValue);
  ++next;
 }
 if (This.boardSerial.isNull())
  throw SysException("TopicInquiry: Serial not provided");
}

void
BoardInquiryRequest::perform()
{
 DBConnection& connection=*This.connectionPtr;
 if (This.boardSerial.isNull())
  throw SysException("BoardInquiry: Serial not provided");
 This.boardData.retrieve(connection,This.boardSerial);
 This.boardData.retrieveTopics(connection);
}

void
BoardInquiryRequest::generateXML(xmlDocPtr document)
{
 DBConnection& connection=*This.connectionPtr;
 BoardData& boardData=This.boardData;
 String boardSerial(boardData.getSerial().toString());
 String description(boardData.getDescription());

 xmlNode* rootElement=xmlDocGetRootElement(document);
 xmlNode* boardElement=xmlNewChild(rootElement,NULL,
  BAD_CAST "BoardInquiry",NULL);

 xmlNewChild(boardElement,NULL,
  BAD_CAST "Serial",
  BAD_CAST boardSerial.c_str());
 xmlNewChild(boardElement,NULL,
  BAD_CAST "Description",
  BAD_CAST description.c_str());

 xmlNode* topicsElement=xmlNewChild(boardElement,NULL,
  BAD_CAST "Topics",NULL);

 BoardData::SerialList::const_iterator next(
  boardData.beginTopicSerialList());
 BoardData::SerialList::const_iterator last(
  boardData.endTopicSerialList());
 while (next!=last)
 {
  const Serial& serial=*next;
  TopicData topicData;
  topicData.retrieve(connection,serial);
  String serialText(serial.toString());
  String userSerial(topicData.getUserSerial().toString());
  String subject(topicData.getSubject());

  xmlNode* topicElement=xmlNewChild(topicsElement,NULL,
   BAD_CAST "Topic",NULL);

  xmlNewChild(topicElement,NULL,
   BAD_CAST "Serial",
   BAD_CAST serialText.c_str());
  xmlNewChild(topicElement,NULL,
   BAD_CAST "UserSerial",
   BAD_CAST userSerial.c_str());
  xmlNewChild(topicElement,NULL,
   BAD_CAST "Subject",
   BAD_CAST subject.c_str());
  ++next;
 }
}

TopicInquiryRequest::TopicInquiryRequest(
 DBConnection& connection,
 QueryRequest& request)
{
 This.connectionPtr=&connection;
 QueryRequest::ElementList::const_iterator next(
  request.elementListBegin());
 QueryRequest::ElementList::const_iterator last(
  request.elementListEnd());
 while (next!=last)
 {
  String elementName(next->getName());
  String elementValue(next->getValue());
  if (elementName!="Serial")
  {
   String text("Invalid request element '");
   text.append(elementName);
   text.append("'");
   throw SysException(text);
  }
  else
   This.topicSerial.fromString(elementValue);
  ++next;
 }
 if (This.topicSerial.isNull())
  throw SysException("TopicInquiry: Serial not provided");
}

void
TopicInquiryRequest::perform()
{
 DBConnection& connection=*This.connectionPtr;
 if (This.topicSerial.isNull())
  throw SysException("TopicInquiry: Serial not provided");
 This.topicData.retrieve(connection,This.topicSerial);
}

void
TopicInquiryRequest::generateXML(xmlDocPtr document)
{
 DBConnection& connection=*This.connectionPtr;
 TopicData& topicData=This.topicData;
 String topicSerial(topicData.getSerial().toString());
 String boardSerial(topicData.getBoardSerial().toString());
 String userSerial(topicData.getUserSerial().toString());
 String subject(topicData.getSubject());

 xmlNode* rootElement=xmlDocGetRootElement(document);
 xmlNode* topicElement=xmlNewChild(rootElement,NULL,
  BAD_CAST "TopicInquiry",NULL);

 xmlNewChild(topicElement,NULL,
  BAD_CAST "Serial",
  BAD_CAST topicSerial.c_str());
 xmlNewChild(topicElement,NULL,
  BAD_CAST "BoardSerial",
  BAD_CAST boardSerial.c_str());
 xmlNewChild(topicElement,NULL,
  BAD_CAST "UserSerial",
  BAD_CAST userSerial.c_str());
 xmlNewChild(topicElement,NULL,
  BAD_CAST "Subject",
  BAD_CAST subject.c_str());

 xmlNode* postsElement=xmlNewChild(topicElement,NULL,
  BAD_CAST "Posts",NULL);

 TopicData::SerialList::const_iterator next(
  topicData.beginPostSerialList());
 TopicData::SerialList::const_iterator last(
  topicData.endPostSerialList());
 while (next!=last)
 {
  const Serial& serial=*next;
  PostData postData;
  postData.retrieve(connection,serial);
  String serialText(serial.toString());
  String userSerial(postData.getUserSerial().toString());
  String subject(postData.getSubject());
  String content(postData.getContent());

  xmlNode* postElement=xmlNewChild(postsElement,NULL,
   BAD_CAST "Post",NULL);

  xmlNewChild(postElement,NULL,
   BAD_CAST "Serial",
   BAD_CAST serialText.c_str());
  xmlNewChild(postElement,NULL,
   BAD_CAST "UserSerial",
   BAD_CAST userSerial.c_str());
  xmlNewChild(postElement,NULL,
   BAD_CAST "Subject",
   BAD_CAST subject.c_str());
  xmlNewChild(postElement,NULL,
   BAD_CAST "Content",
   BAD_CAST content.c_str());
  ++next;
 }
}

PostInquiryRequest::PostInquiryRequest(
 DBConnection& connection,QueryRequest& request)
{
 This.connectionPtr=&connection;
 QueryRequest::ElementList::const_iterator next(
  request.elementListBegin());
 QueryRequest::ElementList::const_iterator last(
  request.elementListEnd());
 while (next!=last)
 {
  String elementName(next->getName());
  String elementValue(next->getValue());
  if (elementName!="Serial")
  {
   String text("Invalid request element '");
   text.append(elementName);
   text.append("'");
   throw SysException(text);
  }
  else
   This.postSerial.fromString(elementValue);
  ++next;
 }
 if (This.postSerial.isNull())
  throw SysException("PostInquiry: Serial not provided");
}

void
PostInquiryRequest::perform()
{
 DBConnection& connection=*This.connectionPtr;
 if (This.postSerial.isNull())
  throw SysException("PostInquiry: Serial not provided");
 This.postData.retrieve(connection,This.postSerial);
}

void
PostInquiryRequest::generateXML(xmlDocPtr document)
{
 PostData& postData=This.postData;
 String postSerial(postData.getSerial().toString());
 String topicSerial(postData.getTopicSerial().toString());
 String userSerial(postData.getUserSerial().toString());
 String subject(postData.getSubject());
 String content(postData.getContent());

 xmlNode* rootElement=xmlDocGetRootElement(document);
 xmlNode* postElement=xmlNewChild(rootElement,NULL,
  BAD_CAST "PostInquiry",NULL);

 xmlNewChild(postElement,NULL,
  BAD_CAST "Serial",
  BAD_CAST postSerial.c_str());
 xmlNewChild(postElement,NULL,
  BAD_CAST "TopicSerial",
  BAD_CAST topicSerial.c_str());
 xmlNewChild(postElement,NULL,
  BAD_CAST "UserSerial",
  BAD_CAST userSerial.c_str());
 xmlNewChild(postElement,NULL,
  BAD_CAST "Subject",
  BAD_CAST subject.c_str());
 xmlNewChild(postElement,NULL,
  BAD_CAST "Content",
  BAD_CAST content.c_str());
}
