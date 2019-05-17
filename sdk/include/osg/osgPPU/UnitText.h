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

#ifndef _C_UNITS_TEXT_HUD_H_
#define _C_UNITS_TEXT_HUD_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgText/Text>

#include <osgPPU/Export.h>
#include <osgPPU/UnitInOut.h>

namespace osgPPU
{
    //! Same as UnitInOut but renders text onto the output.
    /**
    * The text is displayed in 2D ortho mode. This class is wrapper for the 
    * osgText::Text class, so check it for more information about the text support.
    **/
    class OSGPPU_EXPORT UnitText : public UnitInOut
    {
        public:
            META_Node(osgPPU, UnitText);

            //! Create default ppfx 
            UnitText();
            UnitText(const UnitText&, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            
            //! Release it and used memory
            virtual ~UnitText();
            
            //! Initialze the default postprocessing unit 
            virtual void init();
        
            //! Set size of the characters (relative to viewport.width / 640)
            inline void setSize(float size)
            {
                mSize= size;
                if (getViewport())
                    mText->setCharacterSize(mSize * (float(getViewport()->width()) / 640.0), 1.0);
            }
            inline float getSize() const { return mSize; }

            //! Set text position in 2D coordinates        
            inline void setPosition(float x, float y) { mText->setPosition(osg::Vec3(x,y,0)); }

            //! Text Unit does work as a simple bypass, hence return here always the input
            inline osg::Texture* getOrCreateOutputTexture(int mrt = 0) { return mInputTex[mrt].get(); }

            //! Get text assigned with this unit
            osgText::Text& getText() { return *mText; }
            const osgText::Text& getText() const { return *mText; }

            //! Set string drawed by the text
            void setText(const std::string& txt) { mText->setText(txt); }

            //! Set the text pointer used for the rendering
            void setText(osgText::Text* text);

        protected:

            //! Text class holder
            osg::ref_ptr<osgText::Text> mText;

            //! Size of the font
            float mSize;

            virtual bool noticeBeginRendering (osg::RenderInfo&, const osg::Drawable* ) ;


    };

}; // end namespace

#endif
