/***************************************************************************
*   Copyright (c) 2010   Art Tevs                                         *
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
#ifndef _C_OSGPPU_CAMERA__H_
#define _C_OSGPPU_CAMERA__H_

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/Export.h>
#include <osgPPU/Unit.h>
#include <osg/Camera>
#include <osg/FrameBufferObject>

namespace osgPPU
{

	//! Camera class which can be used with osgPPU
	/**
	* A camera class which acts as a wrapper for standard osg::Camera
	* and includes useful methods for proper use of osgPPU.
	*
	* NOTE: You do not have to use this class, this is just a wrapper for simple usage.
	**/
	class OSGPPU_EXPORT Camera : public osg::Camera
	{
		public:

			/**
			* Resize camera's viewport. This will also resize camera's attachments if such exists.
			* Call this method when you have resized your window and wish to update the camera to
			* the new size.
			**/
			static void resizeViewport(int x, int y, int width, int height, osg::Camera* camera);
	};

	//! Derived class from osg's FBO implementation in order to allow marking FBO as dirty
	/**
	* This is a workaround implementation of osg::FrameBufferObject giving a possibility
	* to mark the FBO as dirty. This is required when you made changes to FBOs attachments
	* and you want to propagate them to the FBO.
	*/
	class OSGPPU_EXPORT FrameBufferObject : public osg::FrameBufferObject
	{
	public:
		void dirty() { dirtyAll(); }
	};

};

#endif
