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

#ifndef _C_UNIT_H_
#define _C_UNIT_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osg/Texture>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/BufferObject>
#include <osg/FrameBufferObject>

#include <osgPPU/Export.h>
#include <osgPPU/ColorAttribute.h>

#define OSGPPU_VIEWPORT_WIDTH_UNIFORM "osgppu_ViewportWidth"
#define OSGPPU_VIEWPORT_HEIGHT_UNIFORM "osgppu_ViewportHeight"
#define OSGPPU_VIEWPORT_INV_WIDTH_UNIFORM "osgppu_InvViewportWidth"
#define OSGPPU_VIEWPORT_INV_HEIGHT_UNIFORM "osgppu_InvViewportHeight"

namespace osgPPU
{

//! Abstract base class of any unit
/**
 * Units represents renderable units of the osgPPU library.
 * Each unit has its own functionality. Units can be setted up
 * as a graph, where child units always get as input the output of the
 * parental units. Units has to be a subgraph of the Processor otherwise
 * the rendering wouldn't work, because processor do setup the units in a proper way.
 *
 **/
class OSGPPU_EXPORT Unit : public osg::Group {
    public:

        META_Node(osgPPU, Unit);

        typedef std::map<int, osg::ref_ptr<osg::Texture> > TextureMap;
        typedef std::vector<unsigned int> IgnoreInputList;
        typedef std::map<osg::ref_ptr<Unit>, std::pair<std::string, unsigned int> > InputToUniformMap;
        typedef std::map<int, osg::ref_ptr<osg::PixelDataBufferObject> > PixelDataBufferObjectMap;

        /**
        * Empty constructor. The unit will be initialized with default values.
        **/
        Unit();

        /**
         * Copy constructor.
        **/
        Unit(const Unit&, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

        /**
        * Release used memory by the ppu.
        **/
        virtual ~Unit();

        /**
        * Set an input from the given parent to be linked with the given
        * uniform name. This is required to automatically setup uniforms for
        * input textures of the assigned shader, which is based on the index
        * of the given parent unit in the parent list.
        * The type of the given uniform will be equivalent to the type of the
        * input texture (e.g. SAMPLER_2D = Texture2D).
        * @param parent Pointer to the parent which output to use
        * @param uniform Name of the uniform to use to bind the texture to
        * @param add if true will add the given parent to the parent list
        *             (same as calling parent->addChild()) [default=false]
        * @return true if uniform is set or false otherwise
        **/
        bool setInputToUniform(Unit* parent, const std::string& uniform, bool add = false);

        /**
        * Set input texture of this unit as uniform of this unit.
        * @param index Input texture index to be used as input
        * @param uniform Name of the uniform
        **/
        bool setInputToUniform(int index, const std::string& uniform);

        /**
        * Remove an assigned parent output uniform. @see assignParentToUniform()
        * @param uniform Name of the uniform
        * @param del Should this unit be removed from the child list of the parent
        * connected with the given uniform [default=false]
        **/
        void removeInputToUniform(const std::string& uniform, bool del = false);

        /**
        * Remove an assigned parent output uniform. @see assignParentToUniform()
        * @param parent Pointer to the parent node
        * @param del Should this unit be removed from the child list of this parent [default=false]
        **/
        void removeInputToUniform(Unit* parent, bool del = false);

        /**
        * Get the map which maps uniform to input units
        **/
        inline const InputToUniformMap& getInputToUniformMap() const { return mInputToUniformMap; }

        /**
         * Return an input texture of a certain index.
         * @param inputIndex Index of the input texture (index is equal to the texture unit)
        **/
        inline osg::Texture* getInputTexture(int inputIndex) { return mInputTex[inputIndex].get(); }

        /**
        * Return complete index to texture mapping
        **/
        inline const TextureMap& getInputTextureMap() const {return mInputTex;}

        /**
        * Get output texture of certain MRT index.
        * NOTE: If you haven't initialized the Unit before calling this method
        * it might end up in a NULL as output texture. For this purpose do use
        * the getOrCreateOutputTexture().
        **/
        inline osg::Texture* getOutputTexture(int mrt = 0) { return mOutputTex[mrt].get(); }

        /**
        * Return an output texture of the certain MRT index.
        * If there is no output texture for that index is specified, then
        * it will be allocated. The method should be overwriten by the derived
        * Units if they use any Output texture. Otherwise the result is always
        * the same as from getOutputTexture().
        **/
        virtual osg::Texture* getOrCreateOutputTexture(int mrt = 0) { return getOutputTexture(mrt); }

        /**
        * Get mrt index to texture mapping
        **/
        inline const TextureMap& getOutputTextureMap() const {return mOutputTex;}

        /**
        * If you like that a unit doesn't use a certain input you can specify its index here.
        * This allows to place units on certain positions in the unit graph without using its
        * parents as inputs.
        **/
        void setIgnoreInput(unsigned int index, bool b = true);

        /**
        * Get input ignorance map which list all indices of input data which are ignored
        **/
        inline const IgnoreInputList& getIgnoreInputList() const { return mIgnoreList; }

        /**
        * Check whenever the input of the given index is ignored or not
        **/
        bool getIgnoreInput(unsigned int index) const;

        /**
        * Initialze the unit. This method should be overwritten by the
        * derived classes to support non-standard initialization routines.
        * If an unit is marked as dirty this method will be used to resetup the unit.
        * Hence do provide a "reinitialable"-code here ;-)
        **/
        virtual void init();

        /**
        * Update the unit. Call this method every time you want to update
        * the unit. It is a good idea to call this method every frame otherwise
        * the behaviour of the unit might be unpredictable.
        **/
        virtual void update();

        /**
        * Set viewport which is used for this Unit while rendering. Setting any viewport will force it
        * to be used. If no viewport is set, then either input texture size is used as viewport or 
        * processors camera viewport is used as input. 
        **/
        void setViewport(osg::Viewport* vp);

        /**
        * Get viewport of this unit
        **/
        inline osg::Viewport* getViewport() const { return mViewport.get(); }

        /**
         * Activate or deactive the ppu. An active ppu is updated during the update
         * of the post processor.
         * @param b True to activate, false to deactive
        **/
        inline void setActive(bool b) { mbActive = b; }

        /**
        * Check if the Unit's active flag
        **/
        inline bool getActive() const { return mbActive; }

        /**
         * Change drawing position and size of this ppu by using the
         * new frustum planes in the orthogonal projection matrix.
         * This changes the projection matrix,
         * therefor it is better not to change this parameters until you really
         * need this. If you just want to place the ppu on another position, then just
         * play with the viewport.
        **/
        void setRenderingFrustum(float left, float top, float right, float bottom);

        /**
        * Set index of an input texture which size is used as reference
        * for the viewport size. The viewport size will be changed according
        * to the texture size. If you change the input texture the size will
        * be also changed. Specify -1 if you do not want to have this behaviour.
        * If -1, then by next change of the input texture the viewport size
        * will not be changed.
        **/
        void setInputTextureIndexForViewportReference(int index);

        /**
        * Get index of the input texture which dimension is used for setting up the viewport.
        **/
        inline int getInputTextureIndexForViewportReference() const { return mInputTexIndexForViewportReference; }

        /**
        * Mark this unit as dirty. This will force it to resetup its data
        * on next update. Also every child unit will be marked as dirty. This yields of
        * reinitialization of children units on next update method too.
        **/
        virtual void dirty();

        /**
        * Checks whenever the unit is marked as dirty or not.
        **/
        inline bool isDirty() const { return mbDirty; }

        /**
        * Get geode to which the unit's drawables are attached. The geodes
        * are used to render the unit.
        **/
        osg::Geode* getGeode() { return mGeode.get(); }
        inline const osg::Geode* getGeode() const { return mGeode.get(); }

        /**
        * Setup children nodes if they are connected by a barrier node.
        * This method don't need to be called outside of the unit. The method
        * is placed here to allow acess to it from the derived units.
        **/
        void setupBlockedChildren();

        /**
        * Set a new color attribute for this unit. This will replace the color attribute
        * if it is already set. The color attribute can be used to bind a color to
        * an unit. Very useful for blending units.
        **/
        void setColorAttribute(ColorAttribute* ca);

        /**
        * Get current color attribute of the unit.
        **/
        ColorAttribute* getColorAttribute() { return mColorAttribute.get(); }
        inline const ColorAttribute* getColorAttribute() const { return mColorAttribute.get(); }

        /**
        * Return a PixelDataBufferObject associated with the input texture. 
        * If using of pbos is deactivated, then undefined result might be given back. 
        **/
        inline const osg::PixelDataBufferObject* getInputPBO(int inputIndex) { return mInputPBO[inputIndex].get(); }
        inline const osg::PixelDataBufferObject* getOutputPBO(int mrt) { return mOutputPBO[mrt].get(); }

        inline const PixelDataBufferObjectMap& getInputPBOMap() const { return mInputPBO; }
        inline const PixelDataBufferObjectMap& getOutputPBOMap() const { return mOutputPBO; }

        /**
        * Specify a flag whenever input textures have to be mapped to the input PixelDataBufferObjects.
        * If enabled, data from the input texture will be written to the according input PBO on
        * each apply. This gives you the possibility of using the data stored in the buffer externally.
        * NOTE: A PixelDataBufferObject resides also on the GPU as the texture does. Hence reading/writing
        *       of data in/to PBO isn't that costly, because data is copied in the video memory.
        **/
        void setUsePBOForInputTexture(int index, bool use);
        inline bool getUsePBOForInputTexture(int index) { return mInputPBO[index].valid(); }

        void setUsePBOForOutputTexture(int mrt, bool use);
        inline bool getUsePBOForOutputTexture(int mrt) { return mOutputPBO[mrt].valid(); }

        /** 
        * Push current FBO, so that it can safely be overwritten.
        * Derived classes and its subclasses get use of this method.
        **/
        inline void pushFrameBufferObject(osg::State& state)
        {
            GLint fbo = 0;
            glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo);
            mPushedFBO[state.getContextID()] = fbo;
        }

        /** 
        * Restore last used FrameBufferObject back. This will restore
        * the FBO pushed before with pushFrameBufferObject method.
        **/
        inline void popFrameBufferObject(osg::State& state)
        {
            const osg::GLExtensions* ext = osg::GLExtensions::Get(state.getContextID(), true);
            ext->glBindFramebuffer(GL_FRAMEBUFFER_EXT, mPushedFBO[state.getContextID()]);
        }

        /**
        * Unit traverse function used by node visitors when updating or rendering the unit.
        * Units are traversed in the DFS manner. However, a unit is get only first applied
        * on the visitor, if all its parents has been already traversed. This force every
        * parent to compute its output before a child can start its computation.
        **/
        virtual void traverse(osg::NodeVisitor& nv);

        /**
        * A notify callback can be used by anyone in order to be informed when a unit 
        * is doing special operations, i.e. rendering.
        * Mostly this is used to be able to react on actions of a unit.
        **/
        struct NotifyCallback : public virtual osg::Object
        {
                META_Object (osgPPU, NotifyCallback);

                NotifyCallback(){}
                
                NotifyCallback(const NotifyCallback&, const osg::CopyOp&){}

                virtual void operator()(osg::RenderInfo&, const Unit*) const{};
        };

        /**
        * Set before rendering notify callback. Callback is executed before main 
        * drawable of a unit is drawed.
        **/
        inline void setBeginDrawCallback(NotifyCallback* cb) { _notifyBeginDrawCallback = cb; }
        inline NotifyCallback* getBeginDrawCallback() { return _notifyBeginDrawCallback; }
        inline const NotifyCallback* getBeginDrawCallback() const { return _notifyBeginDrawCallback; }

        /**
        * Set after rendering notify callback. Callback is executed after main 
        * drawable of a unit is drawed.
        **/
        inline void setEndDrawCallback(NotifyCallback* cb) { _notifyEndDrawCallback = cb; }
        inline NotifyCallback* getEndDrawCallback() { return _notifyEndDrawCallback; }
        inline const NotifyCallback* getEndDrawCallback() const { return _notifyEndDrawCallback; }

    protected:

        /**
        * This draw callback is used to setup correct drawing
        * of the unit's drawable. The callback is setted up automatically,
        * hence you don't need to do anything.
        **/
        struct DrawCallback : public osg::Drawable::DrawCallback
        {
            DrawCallback(Unit* parent) : osg::Drawable::DrawCallback(), _parent(parent) {}
            ~DrawCallback() {}

            void drawImplementation (osg::RenderInfo& ri, const osg::Drawable* dr) const;

            Unit* _parent;
        };

        /**
        * Simple draw callback, which just do nothing. It will be executed by every unit 
        * even which just bypass the data. This is needed in order to be able to catch up
        * the moment when a unit is to be computing the output. So virtually speaking
        * every unit does compute the output, however only UnitInOut do really render something
        * the rest just do nothing.
        **/
        struct EmptyDrawCallback : public osg::Drawable::DrawCallback
        {
            EmptyDrawCallback(Unit* parent) : osg::Drawable::DrawCallback(), _parent(parent) {}
            void drawImplementation (osg::RenderInfo& ri, const osg::Drawable* dr) const;

            Unit* _parent;
        };

        /**
        * Use this method in the derived classes to implement and update some unit
        * specific uniforms. The base class do only update uniforms like viewport size
        * or, if defined, input texture indices.
        **/
        virtual void updateUniforms();

        /**
        * Setup the input textures based on the parents. Each unit has to setup its
        * input textures properly. This method do scan for all parents up to the Processor
        * and use the output textures of that parents units and the processor as
        * input to this unit. Call this method from derived units to setup inputs properly.
        **/
        virtual void setupInputsFromParents();

        //! Method to let the unit know that the rendering will now beginns, if returned false, then drawable is not rendered
        virtual bool  noticeBeginRendering (osg::RenderInfo&, const osg::Drawable* ) { return true; }

        //! Let the unit know that the drawing is done.
        virtual void  noticeFinishRendering(osg::RenderInfo&, const osg::Drawable* ) {};

        //! Notice derived classes, that viewport size has changed.
        virtual void noticeChangeViewport(osg::Viewport* newViewport) {}

        //! Notice derived classes, when input texture has changed.
        virtual void noticeChangeInput() {}

        //! Assign the input texture to the quad object
        virtual void assignInputTexture();

        //! Assign currently choosen viewport to the stateset
        virtual void assignViewport();

        //! Assign input/output PBOs to according textures
        virtual void assignInputPBO();

        //! Helper function to create screen sized quads
        osg::Drawable* createTexturedQuadDrawable(const osg::Vec3& corner = osg::Vec3(0,0,0),const osg::Vec3& widthVec=osg::Vec3(1,0,0),const osg::Vec3& heightVec=osg::Vec3(0,1,0));

        //! Input texture
        TextureMap  mInputTex;

        //! Output textures
        TextureMap  mOutputTex;

        //! Input pbos of the textures
        PixelDataBufferObjectMap mInputPBO;

        //! Output pbos of the textures
        PixelDataBufferObjectMap mOutputPBO;

        //! List of ignored inputs
        IgnoreInputList mIgnoreList;

        //! Map of the uniform to parent links
        InputToUniformMap mInputToUniformMap;

        //! Here we store a screen sized quad, so it can be used for rendering
        osg::ref_ptr<osg::Drawable> mDrawable;

        //! Projection matrix of the ppu (default: 2D ortho view)
        osg::ref_ptr<osg::RefMatrix> sProjectionMatrix;

        //! Modelview matrix of the ppu (default: identity matrix)
        osg::ref_ptr<osg::RefMatrix> sModelviewMatrix;

        //! Store here the viewport of the camera, to which one this PPUs are applied
        osg::ref_ptr<osg::Viewport> mViewport;

        //! This geode is used to setup the unit's drawable
        osg::ref_ptr<osg::Geode> mGeode;

        //! Color attribute for fast direct access
        osg::ref_ptr<ColorAttribute> mColorAttribute;

        //! Is the unit dirty
        bool mbDirty;

        //! Index of the input texture which size is used as viewport
        int mInputTexIndexForViewportReference;

        //! Pushed FBOs
        mutable osg::buffered_value<GLuint> mPushedFBO;

        void printDebugInfo(const osg::Drawable* dr);

    private:
        bool mbActive;

        // Separate both folowing variables to allow update and cull traversal in different threads
        bool mbUpdateTraversed; // requires to check whenever unit was already traversed by update visitor
        bool mbCullTraversed; // requires to check whenever unit was already traversed by cull visitor

        osg::ref_ptr<NotifyCallback> _notifyBeginDrawCallback;
        osg::ref_ptr<NotifyCallback> _notifyEndDrawCallback;

        // it is good to have friends
        friend class Processor;
        friend class Pipeline;
        friend class CleanUpdateTraversedVisitor;
        friend class CleanCullTraversedVisitor;
        friend class SetMaximumInputsVisitor;
};

};

#endif
