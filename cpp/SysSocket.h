// System Socket definitions

#ifndef H_SYS_SOCKET
#define H_SYS_SOCKET

#include "Sys.h"
#include "SysException.h"

class Socket
{
// private data
private:
 int socketFileDescriptor;

// public functions
public:
 Socket()
 {This.socketFileDescriptor=-1; }
 ~Socket();

 bool listen(const String& port);
 bool accept(Socket& clientSocket);

 bool connect(String& host, String& port);
 bool disconnect();

 bool write(String data);
 bool write(byte* dataPtr,uint dataSize);
 bool read(String& to,uint maxSize=1000);
 uint read(void* targetPtr,uint maxSize);
};

class SysSocketException : public SysException
{
// public functions
public:
 SysSocketException(int errorNumber);
};

#endif
