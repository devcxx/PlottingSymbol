/***************************************************************************
 *   Copyright (c) 2008   Art Tevs                                         *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 3 of        *
 *   the License, or (at your option) any later version.                   *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesse General Public License for more details.                    *
 *                                                                         *
 *   The full license is in LICENSE file included with this distribution.  *
 ***************************************************************************/

#ifndef _C_OSGPPU_EXPORT_H_
#define _C_OSGPPU_EXPORT_H_

#if defined(_MSC_VER)
 #pragma warning( disable : 4244 )
 #pragma warning( disable : 4251 )
 #pragma warning( disable : 4267 )
 #pragma warning( disable : 4275 )
 #pragma warning( disable : 4290 )
 #pragma warning( disable : 4786 )
 #pragma warning( disable : 4305 )
 #pragma warning( disable : 4996 )

#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
    #  if defined( OSGPPU_LIBRARY_STATIC )
    #    define OSGPPU_EXPORT
    #  elif defined( OSGPPU_LIBRARY )
    #    define OSGPPU_EXPORT   __declspec(dllexport)
    #  else
    #    define OSGPPU_EXPORT   __declspec(dllimport)
    #  endif
#else
    #  define OSGPPU_EXPORT
#endif

// set up define for whether member templates are supported by VisualStudio compilers.
#ifdef _MSC_VER
# if (_MSC_VER >= 1300)
#  define __STL_MEMBER_TEMPLATES
# endif
#endif //_MSC_VER

/* Define NULL pointer value */

#ifndef NULL
    #ifdef  __cplusplus
        #define NULL    0
    #else
        #define NULL    ((void *)0)
    #endif
#endif //NULL

#endif //_C_OSGPPU_EXPORT_H_
