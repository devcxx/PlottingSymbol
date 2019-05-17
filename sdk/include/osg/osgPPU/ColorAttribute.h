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

#ifndef _C_COLORATTRIBUTE_H_
#define _C_COLORATTRIBUTE_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osg/StateAttribute>
#include <osg/Vec4>
#include <osg/BlendFunc>
#include <osg/NodeVisitor>

#include <osgPPU/Export.h>

namespace osgPPU
{

    //! Stateattribute to work with blending. Can be used for animated blending operations (fadein, fadeout)
    /**
    * This is a class derived from StateAttirbute. It allows to specify blending colors based
    * on the reference time provided with the corresponding node visitor. This class is also 
    * used to setup any unit's geometry to default color (1,1,1,1) which is required for 
    * proper rendering.
    **/
    class OSGPPU_EXPORT ColorAttribute : public osg::StateAttribute
    {
        public:
            META_StateAttribute(osgPPU, ColorAttribute, MATERIAL);

            ColorAttribute();
            ColorAttribute(const ColorAttribute& bm, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            ~ColorAttribute();

            /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
            int compare(const osg::StateAttribute& sa) const
            {
                // Check for equal types, then create the rhs variable
                // used by the COMPARE_StateAttribute_Paramter macros below.
                COMPARE_StateAttribute_Types(ColorAttribute, sa)
    
                // Compare each parameter in turn against the rhs.
                COMPARE_StateAttribute_Parameter(mColor)
                COMPARE_StateAttribute_Parameter(mStartColor)
                COMPARE_StateAttribute_Parameter(mEndColor)
                COMPARE_StateAttribute_Parameter(mStartTime)
                COMPARE_StateAttribute_Parameter(mEndTime)
                COMPARE_StateAttribute_Parameter(mTime)
    
                return 0; // Passed all the above comparison macros, so must be equal.
            }

            /**
            * Apply attribute to the state. This will just call the glColor() function with appropriate
            * values. The corresponding time is updated during the update traversion.
            **/
            void apply(osg::State& state) const;

            /**
            * Set to use GL_BLEND mode, because it is required for this attribute
            **/
            bool getModeUsage(osg::StateAttribute::ModeUsage& usage) const
            {
                usage.usesMode(GL_BLEND);
                return true;
            }

            /** Set start time for the interpolationm operation **/
            void setStartTime(double time) { mStartTime = time; }

            /** Get start interpolation time **/
            double getStartTime() const { return mStartTime; }

            /** Set end time for the interpolationm operation **/
            void setEndTime(double time) { mEndTime = time; }

            /** Get end interpolation time **/
            double getEndTime() const { return mEndTime; }

            /** Set start value of the interpolationm operation **/
            void setStartColor(const osg::Vec4& color) { mStartColor = color; }

            /** Get start interpolation time **/
            const osg::Vec4& getStartColor() const { return mStartColor; }

            /** Set end value of the interpolationm operation **/
            void setEndColor(const osg::Vec4& color) { mEndColor = color; }

            /** Get end interpolation time **/
            const osg::Vec4& getEndColor() const { return mEndColor; }

        private:

            /**
            * Update method to update the color based on the data
            **/
            class UpdateCallback : public osg::StateAttribute::Callback
            {
                void operator()(osg::StateAttribute* sa, osg::NodeVisitor* nv);
            };

            //! Current color in use (default 1,1,1,1)
            osg::Vec4 mColor;

            //! Current time 
            double    mTime;
            
            //! Start time of the alpha transition animation
            double    mStartTime;

            //! End time of the alpha transition animation
            double    mEndTime;

            //! Start value of the color animation
            osg::Vec4 mStartColor;

            //! End color of the color transition animation
            osg::Vec4 mEndColor;

    };

};

#endif

