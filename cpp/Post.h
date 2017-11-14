// post class definitions

#ifndef H_POST
#define H_POST

#include "Sys.h"
#include "Serial.h"
#include "TableBase.h"

class Board
{
// public data
public:
 enum {MaxDescriptionLength=100};
};

class BoardTable: public DBTable
{
// private columns
private:
 static const String name;
 static const String serialColumnName;
 static const String descriptionColumnName;

// public functions
public:
 BoardTable();

 static const String getName()
 { return(BoardTable::name); }
 static const String getSerialColumnName()
 { return(BoardTable::serialColumnName); }
 static const String getDescriptionColumnName()
 { return(BoardTable::descriptionColumnName); }
};

class Topic
{
// public data
public:
 enum {MaxSubjectLength=100};
};

class TopicTable: public DBTable
{
// private columns
private:
 static const String name;
 static const String serialColumnName;
 static const String parentSerialColumnName;
 static const String userSerialColumnName;
 static const String postingDateColumnName;
 static const String postingTimeColumnName;
 static const String subjectColumnName;

// public functions
public:
 TopicTable();

 static const String getName()
 { return(TopicTable::name); }
 static const String getSerialColumnName()
 { return(TopicTable::serialColumnName); }
 static const String getParentSerialColumnName()
 { return(TopicTable::parentSerialColumnName); }
 static const String getUserSerialColumnName()
 { return(TopicTable::userSerialColumnName); }
 static const String getPostingDateColumnName()
 { return(TopicTable::postingDateColumnName); }
 static const String getPostingTimeColumnName()
 { return(TopicTable::postingTimeColumnName); }
 static const String getSubjectColumnName()
 { return(TopicTable::subjectColumnName); }
};

class Post
{
// public types
public:
 enum {MaxSubjectLength=100};
 enum {MaxContentLength=1000};
};

class PostTable: public DBTable
{
// private columns
private:
 static const String name;
 static const String serialColumnName;
 static const String parentSerialColumnName;
 static const String userSerialColumnName;
 static const String postingDateColumnName;
 static const String postingTimeColumnName;
 static const String subjectColumnName;
 static const String contentColumnName;

// public functions
public:
 PostTable();

 static const String getName()
 { return(PostTable::name); }
 static const String getSerialColumnName()
 { return(PostTable::serialColumnName); }
 static const String getParentSerialColumnName()
 { return(PostTable::parentSerialColumnName); }
 static const String getUserSerialColumnName()
 { return(PostTable::userSerialColumnName); }
 static const String getPostingDateColumnName()
 { return(PostTable::postingDateColumnName); }
 static const String getPostingTimeColumnName()
 { return(PostTable::postingTimeColumnName); }
 static const String getSubjectColumnName()
 { return(PostTable::subjectColumnName); }
 static const String getContentColumnName()
 { return(PostTable::contentColumnName); }
};

#endif
