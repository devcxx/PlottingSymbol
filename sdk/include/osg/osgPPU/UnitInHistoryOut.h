/***************************************************************************
 *   Copyright (c) 2009   Art Tevs                                         *
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

#ifndef _C_UNIT_INHISTORYOUT_H_
#define _C_UNIT_INHISTORYOUT_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/Export.h>
#include <osgPPU/UnitInOut.h>
#include <osg/Texture2DArray>

namespace osgPPU
{
    //! NOT IMPLEMENTED (Implementation of history buffer)
    /**
    * NOTE: CURRENTLY NOT IMPLEMENTED
    *
    * A simple implementation of history buffer. Inputs are stored in a ring
    * buffer in an texture array. Output is the texture array and current index and
    * everything neccessary to use the data. 
    **/
    class OSGPPU_EXPORT UnitInHistoryOut : public UnitInOut {
        public:
            META_Node(osgPPU,UnitInHistoryOut);
        
            //! Create default ppfx 
            UnitInHistoryOut();
            UnitInHistoryOut(const UnitInHistoryOut&, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            
            //! Release it and used memory
            virtual ~UnitInHistoryOut();
            
            /**
            * Setup the number of elements in the history buffer.
            * Input frames are stored consecutively in the ring buffer
            * of the given size.
            **/
            inline void setHistorySize(unsigned size) { if (size != _historySize) dirty(); _historySize = size; }

            /**
            * Initialize history buffer implementation.
            **/
            virtual void init();

            /**
            * Update the unit. Ringbuffer is updated and corresponding fbo's are set properly
            **/
            virtual void update();

            /**
            * Return output texture for the specified MRT index.
            * If no such exists, then it will be allocated.
            **/
            virtual osg::Texture* getOrCreateOutputTexture(int mrt = 0);

        protected:
             //! Reassign fbo if output textures changes
            virtual void assignOutputTexture();

            unsigned _historySize;
            std::map<int, std::vector<osg::ref_ptr<osg::FrameBufferObject> > > _fboMap;
            
    };

};

#endif
