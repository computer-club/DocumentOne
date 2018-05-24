// User Post request class code

#include <libxml/tree.h>
#include <ctime>
#include <iomanip>
#include "Sys.h"
#include "SysException.h"
#include "Dbms.h"
#include "UserData.h"
#include "PostData.h"
#include "UserPostRequest.h"

UserPostRequest::UserPostRequest(
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
  if (elementName=="LoginToken")
   This.loginToken=elementValue;
  else if (elementName=="TopicSerial")
   This.topicSerial.fromString(elementValue);
  else if (elementName=="Subject")
   This.subject=elementValue;
  else if (elementName=="Content")
   This.content=elementValue;
  else
  {
   String text("Invalid request element '");
   text.append(elementName);
   text.append("'");
   throw SysException(text);
  }
  ++next;
 }
 if (This.loginToken.empty())
  throw SysException("UserPost: Login Token not provided");
 if (This.topicSerial.isNull())
  throw SysException("UserPost: Topic Serial not provided");
 if (This.subject.empty())
  throw SysException("UserPost: Subject not provided");
 if (This.content.empty())
  throw SysException("UserPost: Content not provided");
}

void
UserPostRequest::perform()
{
 if (This.loginToken.empty())
  throw SysException("UserPost: Login Token not provided");
 if (This.topicSerial.isNull())
  throw SysException("UserPost: Topic Serial not provided");
 if (This.subject.empty())
  throw SysException("UserPost: Subject not provided");
 if (This.content.empty())
  throw SysException("UserPost: Content not provided");

 DBConnection& connection=*This.connectionPtr;

 Serial userSerial=UserData::getUserSerialByToken(
  connection,This.loginToken);
 if (userSerial.isNull())
  throw SysException("UserPost: Login Token not valid");

 UserData userData;
 userData.retrieve(connection,userSerial);
 if (!userData.checkRestrictedAccess(connection,
      Permission::Category::CreatePost))
  throw SysException("UserPost: User does not have permission");

 String dateString;
 String timeString;
 {
  time_t timeRaw;
  time(&timeRaw);
  struct tm* timeInfo=localtime(&timeRaw);
  char dateBuffer[128];
  size_t size=strftime(dateBuffer,sizeof(dateBuffer),"%Y-%m-%d",timeInfo);
  dateString.assign(dateBuffer,size);
  size=strftime(dateBuffer,sizeof(dateBuffer),"%H:%M:%S",timeInfo);
  timeString.assign(dateBuffer,size);
 }

 DBStatement stmt(connection);
 DBResultSet rset(stmt);
 stmt.setInsertInto();
 stmt.addIdentifier(PostTable::getName());
 stmt.addLeftParenthesis();
 stmt.addIdentifier(
  PostTable::getParentSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  PostTable::getUserSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  PostTable::getPostingDateColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  PostTable::getPostingTimeColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  PostTable::getSubjectColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  PostTable::getContentColumnName());
 stmt.addRightParenthesis();
 stmt.addValues();
 stmt.addLeftParenthesis();
 stmt.addLiteral(This.topicSerial.toString());
 stmt.addComma();
 stmt.addLiteral(userSerial);
 stmt.addComma();
 stmt.addLiteral(dateString);
 stmt.addComma();
 stmt.addLiteral(timeString);
 stmt.addComma();
 stmt.addLiteral(This.subject);
 stmt.addComma();
 stmt.addLiteral(This.content);
 stmt.addRightParenthesis();
 stmt.executeQuery(rset);

 stmt.setSelect();
 stmt.addLastInsertId();
 stmt.executeQuery(rset);
 if (rset.fetch())
 {
  String postSerialString;
  rset.get(0,postSerialString);
  This.postSerial.fromString(postSerialString);
 }
}

void
UserPostRequest::generateXML(xmlDocPtr document)
{
 DBConnection& connection=*This.connectionPtr;
 PostData postData;
 postData.retrieve(connection,This.postSerial);
 String postSerial(postData.getSerial().toString());
 String topicSerial(postData.getTopicSerial().toString());
 String userSerial(postData.getUserSerial().toString());
 String subject(postData.getSubject());
 String content(postData.getContent());

 xmlNode* rootElement=xmlDocGetRootElement(document);
 xmlNode* postElement=xmlNewChild(rootElement,NULL,
  BAD_CAST "UserPost",NULL);

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

UserTopicPostRequest::UserTopicPostRequest(
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
  if (elementName=="LoginToken")
   This.loginToken=elementValue;
  else if (elementName=="BoardSerial")
   This.boardSerial.fromString(elementValue);
  else if (elementName=="Subject")
   This.subject=elementValue;
  else if (elementName=="Content")
   This.content=elementValue;
  else
  {
   String text("Invalid request element '");
   text.append(elementName);
   text.append("'");
   throw SysException(text);
  }
  ++next;
 }
 if (This.loginToken.empty())
  throw SysException("UserTopicPost: Login Token not provided");
 if (This.topicSerial.isNull())
  throw SysException("UserTopicPost: Board Serial not provided");
 if (This.subject.empty())
  throw SysException("UserTopicPost: Subject not provided");
 if (This.content.empty())
  throw SysException("UserTopicPost: Content not provided");
}

void
UserTopicPostRequest::perform()
{
 if (This.loginToken.empty())
  throw SysException("UserTopicPost: Login Token not provided");
 if (This.boardSerial.isNull())
  throw SysException("UserTopicPost: Board Serial not provided");
 if (This.subject.empty())
  throw SysException("UserTopicPost: Subject not provided");
 if (This.content.empty())
  throw SysException("UserTopicPost: Content not provided");

 DBConnection& connection=*This.connectionPtr;

 Serial userSerial=UserData::getUserSerialByToken(
  connection,This.loginToken);
 if (userSerial.isNull())
  throw SysException("UserTopicPost: Login Token not valid");

 UserData userData;
 userData.retrieve(connection,userSerial);
 if (!userData.checkRestrictedAccess(connection,
      Permission::Category::CreateTopic))
  throw SysException("UserTopicPost: User does not have permission");

 String dateString;
 String timeString;
 {
  time_t timeRaw;
  time(&timeRaw);
  struct tm* timeInfo=localtime(&timeRaw);
  char dateBuffer[128];
  size_t size=strftime(dateBuffer,sizeof(dateBuffer),"%Y-%m-%d",timeInfo);
  dateString.assign(dateBuffer,size);
  size=strftime(dateBuffer,sizeof(dateBuffer),"%H:%M:%S",timeInfo);
  timeString.assign(dateBuffer,size);
 }

 DBStatement stmt(connection);
 DBResultSet rset(stmt);

 stmt.setInsertInto();
 stmt.addIdentifier(TopicTable::getName());
 stmt.addLeftParenthesis();
 stmt.addIdentifier(
  TopicTable::getParentSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  TopicTable::getUserSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  TopicTable::getPostingDateColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  TopicTable::getPostingTimeColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  TopicTable::getSubjectColumnName());
 stmt.addRightParenthesis();
 stmt.addValues();
 stmt.addLeftParenthesis();
 stmt.addLiteral(This.boardSerial.toString());
 stmt.addComma();
 stmt.addLiteral(userSerial);
 stmt.addComma();
 stmt.addLiteral(dateString);
 stmt.addComma();
 stmt.addLiteral(timeString);
 stmt.addComma();
 stmt.addLiteral(subject);
 stmt.addRightParenthesis();
 stmt.executeQuery(rset);

 stmt.setSelect();
 stmt.addLastInsertId();
 stmt.executeQuery(rset);
 if (rset.fetch())
 {
  String topicSerialString;
  rset.get(0,topicSerialString);
  This.topicSerial.fromString(topicSerialString);
 }

 stmt.setInsertInto();
 stmt.addIdentifier(PostTable::getName());
 stmt.addLeftParenthesis();
 stmt.addIdentifier(
  PostTable::getParentSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  PostTable::getUserSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  PostTable::getPostingDateColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  PostTable::getPostingTimeColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  PostTable::getSubjectColumnName());
 stmt.addComma();
 stmt.addIdentifier(
  PostTable::getContentColumnName());
 stmt.addRightParenthesis();
 stmt.addValues();
 stmt.addLeftParenthesis();
 stmt.addLiteral(This.topicSerial.toString());
 stmt.addComma();
 stmt.addLiteral(userSerial);
 stmt.addComma();
 stmt.addLiteral(dateString);
 stmt.addComma();
 stmt.addLiteral(timeString);
 stmt.addComma();
 stmt.addLiteral(This.subject);
 stmt.addComma();
 stmt.addLiteral(This.content);
 stmt.addRightParenthesis();
 stmt.executeQuery(rset);

 stmt.setSelect();
 stmt.addLastInsertId();
 stmt.executeQuery(rset);
 if (rset.fetch())
 {
  String postSerialString;
  rset.get(0,postSerialString);
  This.postSerial.fromString(postSerialString);
 }
}

void
UserTopicPostRequest::generateXML(xmlDocPtr document)
{
 DBConnection& connection=*This.connectionPtr;
 TopicData topicData;
 topicData.retrieve(connection,This.topicSerial);
 String topicSerial(topicData.getSerial().toString());
 String boardSerial(topicData.getBoardSerial().toString());
 String userSerial(topicData.getUserSerial().toString());
 String subject(topicData.getSubject());

 xmlNode* rootElement=xmlDocGetRootElement(document);
 xmlNode* postElement=xmlNewChild(rootElement,NULL,
  BAD_CAST "UserTopicPost",NULL);

 xmlNewChild(postElement,NULL,
  BAD_CAST "Serial",
  BAD_CAST topicSerial.c_str());
 xmlNewChild(postElement,NULL,
  BAD_CAST "BoardSerial",
  BAD_CAST boardSerial.c_str());
 xmlNewChild(postElement,NULL,
  BAD_CAST "UserSerial",
  BAD_CAST userSerial.c_str());
 xmlNewChild(postElement,NULL,
  BAD_CAST "Subject",
  BAD_CAST subject.c_str());
}
