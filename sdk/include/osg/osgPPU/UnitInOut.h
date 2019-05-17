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

#ifndef _C_UNIT_INOUT_H_
#define _C_UNIT_INOUT_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/Export.h>
#include <osgPPU/Unit.h>
#include <osgPPU/Camera.h>

#define OSGPPU_MIPMAP_LEVEL_UNIFORM "osgppu_MipmapLevel"
#define OSGPPU_MIPMAP_LEVEL_NUM_UNIFORM "osgppu_MipmapLevelNum"
#define OSGPPU_CUBEMAP_FACE_UNIFORM "osgppu_CubeMapFace"
#define OSGPPU_3D_SLICE_NUMBER "osgppu_ZSliceNumber"
#define OSGPPU_3D_SLICE_INDEX "osgppu_ZSliceIndex"

namespace osgPPU
{

    //! Compute output texture based on the assigned shaders and input data
    /**
    * InOut PPU, renders the content of input textures with applied shader 
    * to the output textures. Rendering is done in background, so no information
    * will leak to the frame buffer.
    **/
    class OSGPPU_EXPORT UnitInOut : public Unit {
        public:
            META_Node(osgPPU,UnitInOut);
        
            //! Mapping of MRT to ZSlice
            typedef std::map<unsigned int, unsigned int> OutputSliceMap;

            //! Create default ppfx 
            UnitInOut();
            UnitInOut(const UnitInOut&, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            
            //! Release it and used memory
            virtual ~UnitInOut();
            
            //! Initialze the default Processor unit
            virtual void init();
            
            /**
            * Get framebuffer object used by this ppu. 
            **/
            inline FrameBufferObject* getFrameBufferObject() { return mFBO.get(); }

            /**
            * UnitInOut can also be used to bypass the input texture to the output
            * and perform rendering on it. This is different from the UnitBypass which
            * does not perform any rendering but bypasses the data. 
            * Specify here the index of the input unit,
            * to bypass the input to the output.
            * @param index Index of an input unit to bypass to output. Specify -1, to 
            * disable this feature.
            **/
            void setInputBypass(int index);

            /**
            * Get bypassed input texture index.
            **/
            inline int getInputBypass() const { return mBypassedInput; }

            /**
            * Set face index to use when rendering to a cubemap
            **/
            inline void setOutputFace(unsigned int face) { mOutputCubemapFace = face; dirty(); }
 
            /**
            * Get face index used when rendering to a cubemap
            **/
            inline unsigned int getOutputFace() const { return mOutputCubemapFace; }

            /**
            * Set slice index which is used to render the output to.
            * These settings have an effect only when using 3D textures as output.
            * The given slice will be defined for each MRT output.
            * @param slice Index of the slice (z-offset) to render the results to 
            * @param mrt MRT index of the output to be rendered to the given slice. This allows 
            * to specify which output to connect with which slice
            **/
            inline void setOutputZSlice(unsigned int slice, unsigned int mrt = 0) { mOutputZSlice[mrt] = slice; dirty(); }

            /**
            * Get Z slice which is used to render the output to if 3D
            * texture is used as output.
            **/
            inline unsigned int getOutputZSlice(unsigned int mrt) { return mOutputZSlice[mrt]; }

            /**
            * Get the mapping between MRT and Z-Offset slices. @see setOutputZSlice()
            **/
            inline const OutputSliceMap& getOutputZSliceMap() const { return mOutputZSlice; }

            /**
            * Specify the depth of the output texture when using a 3D or layered texture 
            * as output texture. Is a 2D texture is used as output, then this value specifies
            * the amount of MRT (multiple render targets) which has to be used by this unit.
            * 
            * NOTE: Currently there is no support to specify slices of different 3D or layered textures as MRTs
            **/
            void setOutputDepth(unsigned int depth);

            /**
            * Get depth of the output texture. The returned value is the value which 
            * you specify by the setOutputDepth() method and might be different to the 
            * real output texture depth. 
            **/
            inline unsigned int getOutputDepth() const { return mOutputDepth; }

            /**
            * Type of the texture. The types can be used to specify the type 
            * of the output texture of the UnitInOut units.
            **/
            enum TextureType
            {
                //! Texture is a osg::Texture2D
                TEXTURE_2D,

                //! Texture is a osg::TextureCubeMap
                TEXTURE_CUBEMAP,

                //! 3D texture is used of the output
                TEXTURE_3D,

                //! Use 2D texture array
                TEXTURE_2D_ARRAY,

                //! Use a texture rectangle
                TEXTURE_RECTANGLE
            };

            /**
            * Specify the type of the output texture. The next call on
            * getOrCreateOutputTexture() will generate the output texture of the given 
            * type if the output texture wasn't generated before. Use this
            * to implement units based not only on 2D textures.
            * 
            * NOTE: Since the output textures must be all of the same type, 
            * you have to take care about the correct output type. Default type is TEXTURE_2D.
            **/
            inline void setOutputTextureType(TextureType type) { mOutputType = type; dirty(); }

            /**
            * Get the type of the output texture. The type can be specified by 
            * the setOutputTextureType() method. The type must be equal for all output 
            * textures of the same unit.
            **/
            inline TextureType getOutputTextureType() const { return mOutputType; }

            /**
            * Set internal format which will be used by creating the textures. The format
            * specified here will be passed along to the osg::Texture::setInternalFormat()
            * method when creating output textures of a corresponding ppu.
            **/
            void setOutputInternalFormat(GLenum format);
    
            /**
            * Get internal format which is used by the output textures
            **/
            inline GLenum getOutputInternalFormat() const { return mOutputInternalFormat; }

            /**
            * Set an output texture.
            * @param outTex Texture used as output of this ppu 
            * @param mrt MRT (multiple rendering target) index of this output
            **/
            void setOutputTexture(osg::Texture* outTex, int mrt = 0);

            /**
            * Return output texture for the specified MRT index.
            * If no such exists, then it will be allocated.
            **/
            virtual osg::Texture* getOrCreateOutputTexture(int mrt = 0);
            
            /**
            * Set a MRT to texture map for output textures
            **/
            inline void setOutputTextureMap(const TextureMap& map) { mOutputTex = map; dirty();}
    
        protected:

            /**
            * Here the FBO will be applied, so that Unit can render its output to 
            * attached textures. If you overwrite this method in derived class,
            * so you have to take care about FBO handling.
            **/
            virtual bool  noticeBeginRendering (osg::RenderInfo&, const osg::Drawable* ) ;

            /**
            * Drawing is complete. So in our case, we will unbind used FBO and reset it to previous state
            * Derived methods has to take care about correct handling of FBOs!
            **/
            virtual void  noticeFinishRendering(osg::RenderInfo&, const osg::Drawable* );
        
            //! Viewport changed
            virtual void noticeChangeViewport(osg::Viewport*);
    
            //! Reassign fbo if output textures changes
            virtual void assignOutputTexture();

            virtual void assignOutputPBO();

            //! Framebuffer object where results are written
            osg::ref_ptr<FrameBufferObject>    mFBO;    

            //! index of the bypassed input
            int mBypassedInput;

            //! Which face to render cubemaps to
            unsigned int mOutputCubemapFace;

            //! Slice index to which the output is rendered to
            OutputSliceMap mOutputZSlice;

            //! Depth of the 3D texture when used 
            unsigned int mOutputDepth;

            //! Output texture type
            TextureType mOutputType;

            //! Internal format of the output texture
            GLenum mOutputInternalFormat;
    };

};

#endif
