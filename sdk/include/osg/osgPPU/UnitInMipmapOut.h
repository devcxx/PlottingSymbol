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

#ifndef _C_UNIT_INMIPMAPOUT_H_
#define _C_UNIT_INMIPMAPOUT_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/Export.h>
#include <osgPPU/UnitInOut.h>

namespace osgPPU
{
    //! Compute mipmapped output texture based on the input data
    /**
    * UnitInMipmapOut do generate mipmap levels of an output texture
    * by applying the shader to the input data. To generate the mipmapped
    * output the input texture has to be reused. Therefore the output texture
    * will be applied as an input texture. If input textures are already exists
    * the output texture will be applied to the next free slot.
    * Use this Unit to generate mipmaps of an input texture.
    * The shader has to be able to generate correct mipmap levels out of
    * the input data and/or previous levels.
    *
    * You can also setup this unit in that way, that it do generate
    * the mipmaps for the input texture and hence do not use any other output texture,
    * which of course save some memory. 
    **/
    class OSGPPU_EXPORT UnitInMipmapOut : public UnitInOut {
        public:
            META_Node(osgPPU,UnitInMipmapOut);
        
            //! Create default ppfx 
            UnitInMipmapOut();
            UnitInMipmapOut(const UnitInMipmapOut&, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            
            //! Release it and used memory
            virtual ~UnitInMipmapOut();
            
            /**
             * Initialize the UnitInMipmapOut.
             * This will check if the unit is marked as dirty
             * and will therefor setup the input and output texture properly.
             **/
            virtual void init();

            /**
            * Specify the index of the input texture for which the mipmaps should
            * be generated. Set to -1 if you would like to create an independent
            * output texture in which the input texture incl. mipmaps will be stored.
            **/
            inline void setGenerateMipmapForInputTexture(int index)
            {
                mGenerateMipmapInputIndex = index;
                dirty();
            }
    
            /**
            * Get index of an input texture for which the mipmaps are generated. If returns
            * -1 then the output texture is unequal input texture.
            **/
            int getGenerateMipmapForInputTextureIndex() const { return mGenerateMipmapInputIndex; }

            /**
            * Enable or disable the shader for the mipmap generation.
            * If the shader is disabled then the glGenerateMipmapEXT function
            * will be used to generate the mipmaps. Otehrwise the shader is used.
            **/
            void setUseShader(bool use) { if (mUseShader != use) dirty(); mUseShader = use; }
            
            /**
            * Get a flag if a shader is used to generate mipmaps. If returns false,
            * then the hardware mipmap generation is enabled.
            **/
            bool getUseShader() const { return mUseShader; }

        protected:
        
            void enableMipmapGeneration();
            bool noticeBeginRendering (osg::RenderInfo&, const osg::Drawable* );
            void noticeFinishRendering(osg::RenderInfo &renderInfo, const osg::Drawable* drawable);
            void createAndAttachFBOs(osg::Texture* output, int mrt);
        
            std::vector<osg::ref_ptr<FrameBufferObject> > mMipmapFBO;
            std::vector<osg::ref_ptr<osg::Viewport> > mMipmapViewport;
            std::vector<osg::ref_ptr<osg::Drawable> > mMipmapDrawable;

            osg::ref_ptr<osg::RefMatrix> mProjectionMatrix;
            osg::ref_ptr<osg::RefMatrix> mModelviewMatrix;

            int mNumLevels;
            int mGenerateMipmapInputIndex;
            bool mUseShader;
            
            int mOutputWidth;
            int mOutputHeight;
    };

};

#endif
