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

#ifndef _C_UNIT_OUT_IO_H_
#define _C_UNIT_OUT_IO_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/Export.h>
#include <osgPPU/Unit.h>

namespace osgPPU
{
    //! Output the input to the frame buffer instead to the output texture
    /**
    * Pass input texture to the frame buffer. Use this ppu
    * to render results of the previous ppus into the framebuffer. So it is usual that
    * this ppu is applied at the end of the pipeline
    **/
    class OSGPPU_EXPORT UnitOut : public Unit {
        public:
            META_Node(osgPPU,UnitOut);
        
            UnitOut();
            UnitOut(const UnitOut&, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            
            //! Release it and used memory
            virtual ~UnitOut();
            
            //! Initialze the default Processoring unit
            virtual void init();

        protected:
            /**
            * Since UnitOut forces to use no FBO, here we will disable the used FBO.
            * Derived classes has to take care abou that step
            **/
            virtual bool  noticeBeginRendering (osg::RenderInfo&, const osg::Drawable* ) ;

            /** 
            * Disabled FBO will be restored back.
            **/
            virtual void noticeFinishRendering(osg::RenderInfo &renderInfo, const osg::Drawable*);
        
            //! Viewport changed
            virtual void noticeChangeViewport(osg::RenderInfo&) {}

            //! Default FBO instance, so when apply this it FBO with id 0 will be applied
            osg::ref_ptr<osg::FrameBufferObject> mDefaultFBO;
    };
};

#endif
