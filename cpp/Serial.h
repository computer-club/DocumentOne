// Serial definitions

#ifndef H_SERIAL
#define H_SERIAL

#include "Sys.h"

class Serial
{
// private data
private:
 uint64_t value;
 BoolTrue isNullValue;

// public functions
public:
 Serial()
 { This.value=0; }
 Serial(uint value)
 { This.value=value;
   This.isNullValue=false; }
 Serial(String& right)
 { This.value=std::stoi(right);
   This.isNullValue=false; }
 Serial(const Serial& right)
 { This.value=right.value;
   This.isNullValue=right.isNullValue; }

 // set from string
 Serial& operator=(const String& right)
 { This.value=std::stoi(right); }

 // comparison operators
 bool operator==(const Serial& right) const
 { return(This.value==right.value); }
 bool operator!=(const Serial& right) const
 { return(This.value!=right.value); }
 bool operator>(const Serial& right) const
 { return(This.value>right.value); }
 bool operator>=(const Serial& right) const
 { return(This.value>=right.value); }
 bool operator<(const Serial& right) const
 { return(This.value<right.value); }
 bool operator<=(const Serial& right) const
 { return(This.value<=right.value); }

 void setNull()
 { This.value=0;
   This.isNullValue=true; }
 bool isNull()
 { return(This.isNullValue); }

 // convert to string
 const String toString() const
 { String result;
   if (!This.isNullValue)
   {
    char cstr[10];
    size_t len=snprintf(cstr,10,"%ld",This.value);
    return result.assign(cstr,len);
   }
   else
    result.assign("NULL");
   return(result); }
 bool fromString(const String& right)
 { This.value=std::stoi(right);
   This.isNullValue=false; }
};

#endif
