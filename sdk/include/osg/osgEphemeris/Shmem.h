/*
 -------------------------------------------------------------------------------
 | osgEphemeris - Copyright (C) 2007  Don Burns                                |
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

#ifndef __SHMEM_DEF
#define __SHMEM_DEF

#include <string>
#if defined( _DARWIN ) || defined( __APPLE__ )
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#include <osgEphemeris/Export.h>

/** \class Shmem
    \brief A shared memory super class.

    Shmem defines a new() operator that attaches to a shared memory segment
    existing as a memory mapped file.  The name of the file is speicified in 
    a parameter to the new() operator.  Classes deriving from Shmem and using
    the parameter based new() operator, will be allocated as a shared memory
    mapped file.  Contents of classes or structs drived from Shmem will 
    survive process restart and even system reboot.
    */

class OSGEPHEMERIS_EXPORT Shmem 
{
    public :
        /**
          Default Constructor 
          */
	    Shmem( void ) {}
        /** 
          Default Destructor
          */
	    ~Shmem( void ) {}

        /**
          new() operator.
          \param size - Standard size parameter for a C++ new() operator.  Corresponds
                        to the size of the data being allocated.
          \param filename - Name of the file to map to memory in shared space.  This is
                        the name of a file on the file system.  If it does not exist, it
                        will be created and sized appropriately.
          */

	    void *operator new( size_t size, const std::string & filename );
#ifdef WIN32
	    void operator delete( void*, const std::string & filename );
#endif
        /**
          Detaches from shared memory segment, but does not destroy the memory mapped file.
          */
	    void operator delete( void * );

    protected :
        void *start;
    	size_t length;
};


#endif
