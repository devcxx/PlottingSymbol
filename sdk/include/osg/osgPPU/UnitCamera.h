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

#ifndef _C_UNIT_CAMERA_H_
#define _C_UNIT_CAMERA_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/UnitBypass.h>
#include <osg/Camera>

namespace osgPPU
{
    //! Put camera into unit pipeline
    /**
    * This unit is very useful when you want to include camera directly into the unit pipeline.
    * Camera is attached to this unit and its output can be accessed through UnitCameraAttachmentBypass
    * unit. This allows direct including of multiple camera's outputs into the unit pipeline.
    **/
    class OSGPPU_EXPORT UnitCamera : public UnitBypass {
        public:
            META_Node(osgPPU, UnitCamera);
        
            UnitCamera();
            UnitCamera(const UnitCamera& u, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            
            virtual ~UnitCamera();
            
            /**
            * Set camera which output is to be included into the pipeline. 
            * @param camera Camera pointer 
            * @param addAsChild If true camera will be added as child of this unit,
            *       which give the possibility to traversals also on the camera object
            *       without need of including the camera somehwere else in the scene graph.
            **/
            void setCamera(osg::Camera* camera, bool addAsChild = false);

            //! Get camera object used
            osg::Camera* getCamera() { return mCamera.get(); }

        protected:
            osg::ref_ptr<osg::Camera> mCamera;
            bool mUseCameraAsChild;
    };

};

#endif
