// post data object definitions

#ifndef H_POSTDATA
#define H_POSTDATA

#include <list>
#include "Sys.h"
#include "Post.h"

class DBConnection;

class BoardData
{
// public types
public:
 typedef std::list<Serial> SerialList;

// private data
private:
 Serial serial;
 String description;
 SerialList topicSerialList;

 BoolFalse retrieved;
 BoolFalse retrievedTopics;

// public data
public:
 BoardData()
 {}
 BoardData(const BoardData& right);

 void retrieve(
  DBConnection& connection,
  const Serial& serial);
 void retrieveTopics(DBConnection& connection);
 const bool isRetrieved() const
 { return(This.retrieved); }
 const bool isRetrievedTopics() const
 { return(This.retrievedTopics); }

 const Serial& getSerial() const
 { return(This.serial); }
 void setSerial(const Serial& value)
 { This.serial=value; }
 const String& getDescription() const
 { return(This.description); }
 void setDescription(const String& value)
 { This.description=value; }
 
 const SerialList::const_iterator beginTopicSerialList() const
 { return(This.topicSerialList.begin()); }
 const SerialList::const_iterator endTopicSerialList() const
 { return(This.topicSerialList.end()); }
};

class TopicData
{
// public types
public:
 typedef std::list<Serial> SerialList;

// private data
private:
 Serial serial;
 Serial boardSerial;
 Serial userSerial;
 String subject;
 SerialList postSerialList;

 BoolFalse retrieved;

// public data
public:
 TopicData()
 {}
 TopicData(const TopicData& right);

 void retrieve(
  DBConnection& connection,
  const Serial& serial);
 const bool isRetrieved() const
 { return(This.retrieved); }

 const Serial& getSerial() const
 { return(This.serial); }
 const Serial& getBoardSerial() const
 { return(This.boardSerial); }
 const Serial& getUserSerial() const
 { return(This.userSerial); }
 const String& getSubject() const
 { return(This.subject); }
 const size_t getPostCount() const
 { return(This.postSerialList.size()); }

 const SerialList::const_iterator beginPostSerialList() const
 { return(This.postSerialList.begin()); }
 const SerialList::const_iterator endPostSerialList() const
 { return(This.postSerialList.end()); }
};

class PostData
{
// private data
private:
 Serial serial;
 Serial topicSerial;
 Serial userSerial;
 String subject;
 String content;

 BoolFalse retrieved;

// public data
public:
 PostData()
 {}
 PostData(const PostData& right);

 void retrieve(
  DBConnection& connection,
  const Serial& serial);
 const bool isRetrieved() const
 { return(This.retrieved); }

 const Serial getSerial() const
 { return(This.serial); }
 const Serial getTopicSerial() const
 { return(This.topicSerial); }
 const Serial getUserSerial() const
 { return(This.userSerial); }
 const String getSubject() const
 { return(This.subject); }
 const String getContent() const
 { return(This.content); }
};

#endif
