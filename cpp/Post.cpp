// post table code

#include "Sys.h"
#include "User.h"
#include "Post.h"

const String
 BoardTable::name("BOARD");
const String
 BoardTable::serialColumnName("SERIAL");
const String
 BoardTable::descriptionColumnName("DESCRIPTION");

const String
 TopicTable::name("TOPIC");
const String
 TopicTable::serialColumnName("SERIAL");
const String
 TopicTable::parentSerialColumnName("PARENT_SERIAL");
const String
 TopicTable::userSerialColumnName("USER_SERIAL");
const String
 TopicTable::postingDateColumnName("POSTING_DATE");
const String
 TopicTable::postingTimeColumnName("POSTING_TIME");
const String
 TopicTable::subjectColumnName("SUBJECT");

const String
 PostTable::name("POST");
const String
 PostTable::serialColumnName("SERIAL");
const String
 PostTable::parentSerialColumnName("PARENT_SERIAL");
const String
 PostTable::userSerialColumnName("USER_SERIAL");
const String
 PostTable::postingDateColumnName("POSTING_DATE");
const String
 PostTable::postingTimeColumnName("POSTING_TIME");
const String
 PostTable::subjectColumnName("SUBJECT");
const String
 PostTable::contentColumnName("CONTENT");

BoardTable::BoardTable()
{
 This.setTableName(This.getName());
 This.setDescription("Board");
 {
  SerialColumn* columnPtr=new SerialColumn();
  SerialColumn& column=*columnPtr;
  column.setColumnName(This.getSerialColumnName());
  column.setDescription("Serial");
  column.setIndexType(SerialColumn::IndexType::Primary);
  column.setAutoIncrement(true);
  column.setNotNull(true);
  This.addColumn(columnPtr);
 }
 {
  StringColumn* columnPtr=new StringColumn();
  StringColumn& column=*columnPtr;
  column.setColumnName(This.getDescriptionColumnName());
  column.setDescription("Description");
  column.setMaxLength(Board::MaxDescriptionLength);
  This.addColumn(columnPtr);
 }
}

TopicTable::TopicTable()
{
 This.setTableName(This.getName());
 This.setDescription("Topic");
 {
  SerialColumn* columnPtr=new SerialColumn();
  SerialColumn& column=*columnPtr;
  column.setColumnName(This.getSerialColumnName());
  column.setDescription("Serial");
  column.setIndexType(SerialColumn::IndexType::Primary);
  column.setAutoIncrement(true);
  column.setNotNull(true);
  This.addColumn(columnPtr);
 }
 {
  SerialColumn* columnPtr=new SerialColumn();
  SerialColumn& column=*columnPtr;
  column.setColumnName(This.getParentSerialColumnName());
  column.setDescription("Parent Serial");
  column.setForeignKeyReference(
   BoardTable::getName(),
   BoardTable::getSerialColumnName());
  column.setNotNull(true);
  This.addColumn(columnPtr);
 }
 {
  SerialColumn* columnPtr=new SerialColumn();
  SerialColumn& column=*columnPtr;
  column.setColumnName(This.getUserSerialColumnName());
  column.setDescription("User Serial");
  column.setForeignKeyReference(
   UserTable::getName(),
   UserTable::getSerialColumnName());
  column.setNotNull(true);
  This.addColumn(columnPtr);
 }
 {
  DateColumn* columnPtr=new DateColumn();
  DateColumn& column=*columnPtr;
  column.setColumnName(This.getPostingDateColumnName());
  column.setDescription("Posting Date");
  This.addColumn(columnPtr);
 }
 {
  TimeColumn* columnPtr=new TimeColumn();
  TimeColumn& column=*columnPtr;
  column.setColumnName(This.getPostingTimeColumnName());
  column.setDescription("Posting Time");
  This.addColumn(columnPtr);
 }
 {
  StringColumn* columnPtr=new StringColumn();
  StringColumn& column=*columnPtr;
  column.setColumnName(This.getSubjectColumnName());
  column.setDescription("Subject");
  column.setMaxLength(Topic::MaxSubjectLength);
  This.addColumn(columnPtr);
 }
}

PostTable::PostTable()
{
 This.setTableName(This.getName());
 This.setDescription("Post");
 {
  SerialColumn* columnPtr=new SerialColumn();
  SerialColumn& column=*columnPtr;
  column.setColumnName(This.getSerialColumnName());
  column.setDescription("Serial");
  column.setIndexType(SerialColumn::IndexType::Primary);
  column.setAutoIncrement(true);
  column.setNotNull(true);
  This.addColumn(columnPtr);
 }
 {
  SerialColumn* columnPtr=new SerialColumn();
  SerialColumn& column=*columnPtr;
  column.setColumnName(This.getParentSerialColumnName());
  column.setDescription("Parent Serial");
 column.setForeignKeyReference(
  TopicTable::getName(),
  TopicTable::getSerialColumnName());
  column.setNotNull(true);
  This.addColumn(columnPtr);
 }
 {
  SerialColumn* columnPtr=new SerialColumn();
  SerialColumn& column=*columnPtr;
  column.setColumnName(This.getUserSerialColumnName());
  column.setDescription("User Serial");
  column.setForeignKeyReference(
   UserTable::getName(),
   UserTable::getSerialColumnName());
  column.setNotNull(true);
  This.addColumn(columnPtr);
 }
 {
  DateColumn* columnPtr=new DateColumn();
  DateColumn& column=*columnPtr;
  column.setColumnName(This.getPostingDateColumnName());
  column.setDescription("Posting Date");
  This.addColumn(columnPtr);
 }
 {
  TimeColumn* columnPtr=new TimeColumn();
  TimeColumn& column=*columnPtr;
  column.setColumnName(This.getPostingTimeColumnName());
  column.setDescription("Posting Time");
  This.addColumn(columnPtr);
 }
 {
  StringColumn* columnPtr=new StringColumn();
  StringColumn& column=*columnPtr;
  column.setColumnName(This.getSubjectColumnName());
  column.setDescription("Subject");
  column.setMaxLength(Post::MaxSubjectLength);
  This.addColumn(columnPtr);
 }
 {
  StringColumn* columnPtr=new StringColumn();
  StringColumn& column=*columnPtr;
  column.setColumnName(This.getContentColumnName());
  column.setDescription("Content");
  column.setMaxLength(Post::MaxContentLength);
  This.addColumn(columnPtr);
 }
}
