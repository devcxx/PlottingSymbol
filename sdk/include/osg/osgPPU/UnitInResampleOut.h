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

#ifndef _C_UNIT_INRESAMPLEOUT_H_
#define _C_UNIT_INRESAMPLEOUT_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/Export.h>
#include <osgPPU/UnitInOut.h>

namespace osgPPU
{
    //! Same as UnitInOut but do resampling inbetween
    /**
    * Resample the input. This PPU will 
    * render the input data resampled to the output. Next PPU will work 
    * on the resampled one. NOTE: You loose information in your data after 
    * appling this PPU.
    **/
    class OSGPPU_EXPORT UnitInResampleOut : public UnitInOut {
        public:
            META_Node(osgPPU,UnitInResampleOut);
        
            //! Create default ppfx 
            UnitInResampleOut();
            UnitInResampleOut(const UnitInResampleOut&, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            
            //! Release it and used memory
            virtual ~UnitInResampleOut();
            
            //! Set resampling factor
            void setFactorX(float x);
    
            //! Set resampling factor
            void setFactorY(float Y);
    
            //! Get resampling factor
            float getFactorX() const { return mWidthFactor; }
    
            //! Get resampling factor
            float getFactorY() const { return mHeightFactor; }
    
            void init();
            void dirty();

        protected:
            float mWidthFactor, mHeightFactor;
            bool mDirtyFactor;
    };

};

#endif
