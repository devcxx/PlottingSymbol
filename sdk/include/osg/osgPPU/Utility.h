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

#ifndef _C_OSGPPU__H_
#define _C_OSGPPU__H_


#include <osg/Uniform>
#include <osg/Texture>

#include <osgPPU/Export.h>

/**
 * \namespace osgPPU
 * osgPPU module
 **/
namespace osgPPU
{
    /**
    * Utility function to derive source texture format from the internal format.
    * For example GL_RGB16F_ARB corresponds to GL_RGB
    **/
    OSGPPU_EXPORT GLenum createSourceTextureFormat(GLenum internalFormat);

    /**
    * Utility function to derive uniform type based on the given texture.
    **/
    OSGPPU_EXPORT osg::Uniform::Type convertTextureToUniformType(osg::Texture* tex);

    /**
    * Compute memory size in bytes, which is allocated by the texture.
    **/
    OSGPPU_EXPORT unsigned int computeTextureSizeInBytes(osg::Texture* tex);

};

#endif

