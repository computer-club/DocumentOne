// System Exception definition

#ifndef H_SYS_EXCEPTION
#define H_SYS_EXCEPTION

#include "Sys.h"

class SysException {
// private data
private:
 String exceptionText;

// public functions
public:
 SysException();
 SysException(String text);

 void setExceptionText(String value)
 { This.exceptionText=value; }
 const String& getExceptionText()
 { return(This.exceptionText); }

 const char* what() const throw()
 { return(This.exceptionText.c_str()); }
};

#endif
