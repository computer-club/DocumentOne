// System Socket code

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include "Sys.h"
#include "SysSocket.h"

Socket::~Socket()
{
 if (This.socketFileDescriptor!=-1)
  shutdown(This.socketFileDescriptor,SHUT_RDWR);
}

bool
Socket::listen(const String& port)
{
 This.socketFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
 if (This.socketFileDescriptor<0)
  throw SysException("SysSocket::listen failed to create socket");

 struct sockaddr_in serverAddress;
 memset(&serverAddress,0,sizeof(struct sockaddr_in));

 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=INADDR_ANY;
 serverAddress.sin_port=htons(std::atoi(port.c_str()));

 if (bind(This.socketFileDescriptor,
      (struct sockaddr*)&serverAddress,
      sizeof(serverAddress))<0)
  throw SysException("SysSocket::listen failed to bind socket");

 int backlog=5;
 ::listen(This.socketFileDescriptor,backlog);

 return true;
}

bool
Socket::accept(Socket& clientSocket)
{
 struct sockaddr_in clientAddress;
 int clientAddressSize=sizeof(clientAddress);

 clientSocket.socketFileDescriptor=
  ::accept(This.socketFileDescriptor,
   (struct sockaddr*)&clientAddress,
   (socklen_t*)&clientAddressSize);

 if (clientSocket.socketFileDescriptor<0)
  throw SysException("SysSocket::accept failed to create socket");
}

bool
Socket::connect(String& host, String& port)
{
 // create connection hints
 struct addrinfo hints;
 // create result set
 struct addrinfo *resultSet;

 // zero out hints
 memset(&hints,0,sizeof(struct addrinfo));
 // IPv4 & IPv6 are fine
 hints.ai_family=AF_UNSPEC;
 // TCP not UDP
 hints.ai_socktype=SOCK_STREAM;

 uint infoStatus=
  getaddrinfo(host.c_str(),port.c_str(),&hints,&resultSet);

 if (infoStatus!=0) {
  freeaddrinfo(resultSet);
  return false;
 }

 int socketFileDescriptor;
 bool success=false;

 for (struct addrinfo *result=resultSet;
      result!=NULL;
      result=result->ai_next)
 {
  socketFileDescriptor=socket(
   result->ai_family,
   result->ai_socktype,
   result->ai_protocol);
  if (socketFileDescriptor==(-1))
   continue;

  int connectStatus=::connect(
   socketFileDescriptor,
   result->ai_addr,
   result->ai_addrlen);
  if (connectStatus!=(-1)) {
   success=true;
   break;
  }

  shutdown(socketFileDescriptor,SHUT_RDWR);
 }

 freeaddrinfo(resultSet);

 if (success)
  This.socketFileDescriptor=socketFileDescriptor;

 return(success);
}

bool
Socket::disconnect()
{
 if (This.socketFileDescriptor!=-1)
  ::close(This.socketFileDescriptor);
}

bool
Socket::write(String data)
{
 if (This.socketFileDescriptor!=-1) {
  // intercept broken pipe signal
  struct sigaction old_action;
  struct sigaction new_action;
  new_action.sa_handler=SIG_IGN;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_flags=0;
  sigaction(SIGPIPE,&new_action,&old_action);

  // system call for send
  ssize_t result=
   ::send(This.socketFileDescriptor,data.c_str(),data.length(),0);

  // restore old signal behavior
  sigaction (SIGPIPE, &old_action, NULL);

  // handle errors
  if (result==-1)
   throw SysSocketException(errno);
  else
   return true;
 }
 else
  return false;
}

bool
Socket::write(byte* dataPtr,uint dataSize)
{
 if (This.socketFileDescriptor!=-1) {
  // intercept broken pipe signal
  struct sigaction old_action;
  struct sigaction new_action;
  new_action.sa_handler=SIG_IGN;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_flags=0;
  sigaction(SIGPIPE,&new_action,&old_action);

  // system call for send
  ssize_t result=
   ::send(This.socketFileDescriptor,dataPtr,dataSize,0);

  // restore old signal behavior
  sigaction (SIGPIPE, &old_action, NULL);

  // handle errors
  if (result==-1)
   throw SysSocketException(errno);
  else
   return true;
 }
 else
  return false;
}

bool
Socket::read(String& to, uint maxSize)
{
 void* dataPointer=malloc((size_t)maxSize);
 if (This.socketFileDescriptor!=-1) {
  ssize_t result=
   ::recv(This.socketFileDescriptor,dataPointer,maxSize,0);
  if (result==-1) {
   int errorNumber=errno;
   free(dataPointer);
   throw SysSocketException(errorNumber);
   return false;
  }
  else {
   to.assign(String((char*)dataPointer,result));
   free(dataPointer);
   return true;
  }
 }
 else
  return false;
}

uint
Socket::read(void* dataPtr,uint maxSize)
{
 if (This.socketFileDescriptor!=-1) {
  ssize_t result=
   ::recv(This.socketFileDescriptor,dataPtr,maxSize,0);
  if (result==-1) {
   throw SysSocketException(errno);
   return 0;
  }
  else
   return result;
 }
 else 
  return 0;
}

SysSocketException::SysSocketException(int errorNumber)
{
 switch (errorNumber) {
 case EDESTADDRREQ:
  This.setExceptionText("Destination address required");
  break;
 case EMSGSIZE:
  This.setExceptionText("Message too long");
  break;
 case EPROTOTYPE:
  This.setExceptionText("Protocol wrong type for socket");
  break;
 case ENOPROTOOPT:
  This.setExceptionText("Protocol not available");
  break;
 case EPROTONOSUPPORT:
  This.setExceptionText("Protocol not supported");
  break;
 case ESOCKTNOSUPPORT:
  This.setExceptionText("Socket type not supported");
  break;
 case EOPNOTSUPP:
  This.setExceptionText("Operation not supported on transport endpoint");
  break;
 case EPFNOSUPPORT:
  This.setExceptionText("Protocol family not supported");
  break;
 case EAFNOSUPPORT:
  This.setExceptionText("Address family not supported by protocol");
  break;
 case EADDRINUSE:
  This.setExceptionText("Address already in use");
  break;
 case EADDRNOTAVAIL:
  This.setExceptionText("Cannot assign requested address");
  break;
 case ENETDOWN:
  This.setExceptionText("Network is down");
  break;
 case ENETUNREACH:
  This.setExceptionText("Network is unreachable");
  break;
 case ENETRESET:
  This.setExceptionText("Network dropped connection because of reset");
  break;
 case ECONNABORTED:
  This.setExceptionText("Software caused connection abort");
  break;
 case ECONNRESET:
  This.setExceptionText("Connection reset by peer");
  break;
 case ENOBUFS:
  This.setExceptionText("No buffer space available");
  break;
 case EISCONN:
  This.setExceptionText("Transport endpoint is already connected");
  break;
 case ENOTCONN:
  This.setExceptionText("Transport endpoint is not connected");
  break;
 case ESHUTDOWN:
  This.setExceptionText("Cannot send after transport endpoint shutdown");
  break;
 case ETOOMANYREFS:
  This.setExceptionText("Too many references: cannot splice");
  break;
 case ETIMEDOUT:
  This.setExceptionText("Connection timed out");
  break;
 case ECONNREFUSED:
  This.setExceptionText("Connection refused");
  break;
 case EHOSTDOWN:
  This.setExceptionText("Host is down");
  break;
 case EHOSTUNREACH:
  This.setExceptionText("No route to host");
  break;
 case EALREADY:
  This.setExceptionText("Operation already in progress");
  break;
 case EINPROGRESS:
  This.setExceptionText("Operation now in progress");
  break;
 case ESTALE:
  This.setExceptionText("Stale file handle");
  break;
 default:
  This.setExceptionText("Unhandled Exception Encountered");
  break;
 }
}
