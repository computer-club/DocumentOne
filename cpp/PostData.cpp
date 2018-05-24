// post data object code

#include "Sys.h"
#include "PostData.h"
#include "Dbms.h"
#include "Permission.h"

BoardData::BoardData(const BoardData& right)
{
 This.retrieved=right.isRetrieved();
 This.serial=right.getSerial();
 This.description=right.getDescription();

 SerialList::const_iterator next=right.beginTopicSerialList();
 SerialList::const_iterator last=right.endTopicSerialList();
 while (next!=last)
 {
  This.topicSerialList.push_back(*next);
  ++next;
 }
}

void
BoardData::retrieve(
 DBConnection& connection,
 const Serial& serial)
{
 DBStatement stmt(connection);
 DBResultSet rset(stmt);
 stmt.setSelect();
 stmt.addIdentifier(BoardTable::getSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(BoardTable::getDescriptionColumnName());
 stmt.addFrom();
 stmt.addIdentifier(BoardTable::getName());
 stmt.addWhere();
 stmt.addIdentifier(BoardTable::getSerialColumnName());
 stmt.addIsEqualTo();
 stmt.addLiteral(serial);
 stmt.executeQuery(rset);
 if (rset.fetch())
 {
  This.retrieved=true;
  size_t pos=0;
  rset.get(pos++,This.serial);
  rset.get(pos++,This.description);
 }
}

void
BoardData::retrieveTopics(DBConnection& connection)
{
 if (!This.retrieved)
  throw SysException("BoardData not retrieved");
 DBStatement stmt(connection);
 DBResultSet rset(stmt);
 stmt.setSelect();
 stmt.addIdentifier(TopicTable::getSerialColumnName());
 stmt.addFrom();
 stmt.addIdentifier(TopicTable::getName());
 stmt.addWhere();
 stmt.addIdentifier(TopicTable::getParentSerialColumnName());
 stmt.addIsEqualTo();
 stmt.addLiteral(This.serial);
 stmt.executeQuery(rset);
 while (rset.fetch())
 {
  String serialText;
  rset.get(0,serialText);
  This.topicSerialList.push_back(serialText);
 }
}

TopicData::TopicData(const TopicData& right)
{
 This.retrieved=right.isRetrieved();
 This.serial=right.getSerial();
 This.boardSerial=right.getBoardSerial();
 This.userSerial=right.getUserSerial();
 This.subject=right.getSubject();

 SerialList::const_iterator next=right.beginPostSerialList();
 SerialList::const_iterator last=right.endPostSerialList();
 while (next!=last)
 {
  This.postSerialList.push_back(*next);
  ++next;
 }
}

void
TopicData::retrieve(
 DBConnection& connection,
 const Serial& serial)
{
 DBStatement stmt(connection);
 DBResultSet rset(stmt);
 stmt.setSelect();
 stmt.addIdentifier(TopicTable::getSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(TopicTable::getParentSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(TopicTable::getUserSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(TopicTable::getSubjectColumnName());
 stmt.addFrom();
 stmt.addIdentifier(TopicTable::getName());
 stmt.addWhere();
 stmt.addIdentifier(TopicTable::getSerialColumnName());
 stmt.addIsEqualTo();
 stmt.addLiteral(serial);
 stmt.executeQuery(rset);
 if (rset.fetch())
 {
  This.retrieved=true;
  size_t pos=0;
  rset.get(pos++,This.serial);
  rset.get(pos++,This.boardSerial);
  rset.get(pos++,This.userSerial);
  rset.get(pos++,This.subject);
 }
 if (This.retrieved)
 {
  stmt.setSelect();
  stmt.addIdentifier(PostTable::getSerialColumnName());
  stmt.addFrom();
  stmt.addIdentifier(PostTable::getName());
  stmt.addWhere();
  stmt.addIdentifier(PostTable::getParentSerialColumnName());
  stmt.addIsEqualTo();
  stmt.addLiteral(This.serial);
  stmt.executeQuery(rset);
  while (rset.fetch())
  {
   String serialText;
   rset.get(0,serialText);
   This.postSerialList.push_back(serialText);
  }
 }
}

PostData::PostData(const PostData& right)
{
 This.retrieved=right.isRetrieved();
 This.serial=right.getSerial();
 This.topicSerial=right.getTopicSerial();
 This.userSerial=right.getUserSerial();
 This.subject=right.getSubject();
 This.content=right.getContent();
}

void
PostData::retrieve(
 DBConnection& connection,
 const Serial& serial)
{
 DBStatement stmt(connection);;
 DBResultSet rset(stmt);
 stmt.setSelect();
 stmt.addIdentifier(PostTable::getSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(PostTable::getParentSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(PostTable::getUserSerialColumnName());
 stmt.addComma();
 stmt.addIdentifier(PostTable::getSubjectColumnName());
 stmt.addComma();
 stmt.addIdentifier(PostTable::getContentColumnName());
 stmt.addFrom();
 stmt.addIdentifier(PostTable::getName());
 stmt.addWhere();
 stmt.addIdentifier(PostTable::getSerialColumnName());
 stmt.addIsEqualTo();
 stmt.addLiteral(serial);
 stmt.executeQuery(rset);
 if (rset.fetch())
 {
  This.retrieved=true;
  size_t pos=0;
  rset.get(pos++,This.serial);
  rset.get(pos++,This.topicSerial);
  rset.get(pos++,This.userSerial);
  rset.get(pos++,This.subject);
  rset.get(pos++,This.content);
 }
}
