#include <stdio.h>

class BaseClass
{
// private data
private:
 int thingy;

// public functions
public:
 BaseClass()
 { this->setThingy(1); }

 void setThingy(int value)
 { thingy=value; }
 const int getThingy() const
 { return(thingy); }
};

class DerivedClass: public BaseClass
{
// private data
private:
 int thangy;

// public functions
public:
 DerivedClass()
 { this->setThingy(2);
   this->setThangy(2); }

 void setThangy(int value)
 { thangy=value; }
 const int getThangy() const
 { return(thangy); }
};

int main()
{
 DerivedClass* testPtr=new DerivedClass();

 {
  DerivedClass& test=*testPtr;
  printf("1. >%d< >%d<\n",test.getThingy(),test.getThangy());
 }
 {
  BaseClass& test=*testPtr;
  printf("2. >%d<\n",test.getThingy());
 }
 {
  BaseClass* basePtr=testPtr;
  BaseClass& base=*basePtr;
  DerivedClass* testPtrTwo=(DerivedClass *)&base;
  DerivedClass& test=*testPtrTwo;
  printf("3. >%d< >%d<\n",test.getThingy(),test.getThangy());
 }
}
