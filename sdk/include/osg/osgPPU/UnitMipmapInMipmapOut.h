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

#ifndef _C_UNIT_MIPMAPINOUT_H_
#define _C_UNIT_MIPMAPINOUT_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/Export.h>
#include <osgPPU/UnitInOut.h>

namespace osgPPU
{
    //! Same as UnitInOut however do bypass also mipmap levels
    /**
    * The functionality of this unit is similar to the UnitInOut. However this 
    * unit is also capable to bypass mipmap levels. This means that the output texture 
    * will be switched in a mipmap mode and the input textures are passed levelwise
    * to the output texture. 
    *
    * In order that this unit work correctly the input texture and the output should be of the 
    * same dimensions, otherwise non 1:1 matching of mipmap levels is possible.
    **/
    class OSGPPU_EXPORT UnitMipmapInMipmapOut : public UnitInOut {
        public:
            META_Node(osgPPU,UnitMipmapInMipmapOut);
         
            UnitMipmapInMipmapOut();
            UnitMipmapInMipmapOut(const UnitMipmapInMipmapOut&, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            
            //! Release it and used memory
            virtual ~UnitMipmapInMipmapOut();
            
            //! Initialze the Processoring unit
            virtual void init();
            
        protected:

            //! regenerate io mapmapped data structures
            void checkIOMipmappedData();

            bool noticeBeginRendering (osg::RenderInfo&, const osg::Drawable* );

			std::vector<osg::ref_ptr<FrameBufferObject> > mIOMipmapFBO;
            std::vector<osg::ref_ptr<osg::Viewport> > mIOMipmapViewport;
            std::vector<osg::ref_ptr<osg::Drawable> > mIOMipmapDrawable;

            osg::ref_ptr<osg::RefMatrix> mProjectionMatrix;
            osg::ref_ptr<osg::RefMatrix> mModelviewMatrix;
    };

};

#endif
