#ifndef H_SYSTEM
#define H_SYSTEM

// INCLUDES
#include <cstdint>
#include <string>
#include <memory>

// DEFINES
#define This (*this)
#define AutoPtr std::unique_ptr

// TYPEDEFS
typedef std::string String;
typedef std::uint64_t uint64_t;
typedef std::uint32_t uint;
typedef std::uint8_t byte;

// TYPES

class BoolFalse
{
// private data
private:
 bool value;

// public functions
public:
 BoolFalse()
 { This.value=false; }

 BoolFalse& operator=(bool right)
 { This.value=right; return(This); }

 operator bool() const
 { return(This.value); }
};

class BoolTrue
{
// private data
private:
 bool value;

// public functions
public:
 BoolTrue()
 { This.value=true; }

 BoolTrue& operator=(bool right)
 { This.value=right; return(This); }

 operator bool() const
 { return(This.value); }
};

#endif
