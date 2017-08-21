 // Example Main File

#include "Sys.h"

int
main()
{
 AutoPtr<String> printStringPtr(new String("Hello World!\n"));
 String& printString=*printStringPtr;

 printf(printString.c_str());
 printf(printStringPtr->c_str());
}
