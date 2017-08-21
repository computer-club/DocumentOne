// System Exception code

#include "Sys.h"
#include "SysException.h"

SysException::SysException()
 : exceptionText("Unknown Exception")
{}

SysException::SysException(String text)
{
 This.exceptionText=text;
}
