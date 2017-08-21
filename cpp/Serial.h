// Serial definitions

#ifndef H_SERIAL
#define H_SERIAL

#include <string>
#include "Sys.h"

class Serial
{
// private data
private:
 uint64_t value;

// public functions
public:
 Serial()
 { This.value=0; }
 Serial(uint value)
 { This.value=value; }
 Serial(const Serial& right)
 { This.value=right.value; }

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
 { This.value=0; }
 bool isNull()
 { return(This.value==0); }

 // convert to string
 const String toString() const
 { char cstr[10]; snprintf((char *)This.value,10,"%ld"); return String(cstr); }
 bool fromString(const String& right)
 { This.value=std::stoi(right); }
};

#endif
