#ifndef OSGEPHEMERIS_INTTYPES_DEF
#define OSGEPHEMERIS_INTTYPES_DEF

#if defined (__GXX_EXPERIMENTAL_CXX0X__) || defined (_MSC_VER) && MSC_VER >= 1800
  #include <cstdint>
#else
  #ifdef WIN32 

typedef signed __int32          int32_t;
typedef unsigned __int32        uint32_t;

  #  else

#include <stdint.h>

  #endif
#endif

#endif
