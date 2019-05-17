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

#ifndef _C_UNIT_CAMERABUFFER_BYPASS_H_
#define _C_UNIT_CAMERABUFFER_BYPASS_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/UnitBypass.h>

namespace osgPPU
{
    //! Bypass a specified camera attacment into the pipeline
    /**
    * This unit do not perform any rendering, however it do
    * bypass any camera attachment of  camera of the parent processor or unit
    * to its output texture.
    * Per default the unit do bypass the color buffer attachment.
    *
    * This unit has to be placed directly under the processor, so that the unit
    * get access to the processor's attachments.
    *
    * This unit can also bypass a camera attachment of UnitCamera parent unit.
    * For this purpose the unit has to be placed directly under UnitCamera. Camera
    * output will then be bypassed.
    **/
    class OSGPPU_EXPORT UnitCameraAttachmentBypass : public UnitBypass {
        public:
            META_Node(osgPPU,UnitCameraAttachmentBypass);
        
            UnitCameraAttachmentBypass();
            UnitCameraAttachmentBypass(const UnitCameraAttachmentBypass& u, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            
            virtual ~UnitCameraAttachmentBypass();
            
            virtual void init();
        
            /**
            * Setup the buffer component which has to be bypassed.
            **/
            void setBufferComponent(osg::Camera::BufferComponent c);

            /**
            * Get current buffer component
            **/
            const osg::Camera::BufferComponent getBufferComponent() const { return _bufferComponent; }

        protected:
            virtual void setupInputsFromParents();
            osg::Camera::BufferComponent _bufferComponent;

    };
};

#endif
