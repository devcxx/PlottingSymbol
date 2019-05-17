/*
 -------------------------------------------------------------------------------
 | Producer - Copyright (C) 2001-2007  Don Burns                               |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 |                                                                             |
 -------------------------------------------------------------------------------
 */


#ifndef OSGEPHEMERIS_EXPORT_DEF
#define OSGEPHEMERIS_EXPORT_DEF 1

#if defined(_MSC_VER)
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4267 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
    #pragma warning( disable : 4786 )
    #pragma warning( disable : 4541 )
    #pragma warning( disable : 4800 )
    #pragma warning( disable : 4996 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
    #  if defined( OSGEPHEMERIS_LIBRARY_STATIC )
    #    define OSGEPHEMERIS_EXPORT
    #  elif defined( OSGEPHEMERIS_LIBRARY )
    #    define OSGEPHEMERIS_EXPORT   __declspec(dllexport)
    #  else
    #    define OSGEPHEMERIS_EXPORT   __declspec(dllimport)
    #  endif 
#else
    #  define OSGEPHEMERIS_EXPORT
#endif  

// set up define for whether member templates are supported by VisualStudio compilers, i.e for compilers after VS6.0
// also set a define to know when VS6.0 is being used.
#ifdef _MSC_VER
# if (_MSC_VER >= 1300)
#  define __STL_MEMBER_TEMPLATES
#else
#  define __VS6_IMPLEMENTATION
# endif
#endif

/* Define NULL pointer value */

#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifdef WIN32
#  define _WIN32_IMPLEMENTATION 1
#elif defined(__APPLE__) 
#  if defined (__USE_OSX_CGL_IMPLEMENTATION__)
#    define _OSX_CGL_IMPLEMENTATION 1
#  elif defined (__USE_OSX_AGL_IMPLEMENTATION__)
#    define _OSX_AGL_IMPLEMENTATION 1
#  else
#    define _X11_IMPLEMENTATION 1
#    define GLX_GLXEXT_PROTOTYPES 1
#  endif
#else 
#  define _X11_IMPLEMENTATION 1
#endif


#endif
