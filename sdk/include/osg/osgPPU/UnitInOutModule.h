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

#ifndef _C_UNIT_INOUT_MODULE_H_
#define _C_UNIT_INOUT_MODULE_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/Export.h>
#include <osgPPU/UnitInOut.h>
#include <osgDB/DynamicLibrary>

//! Name of the function which is the entry point of the module 
#define OSGPPU_MODULE_ENTRY osgppuInitModule
#define OSGPPU_MODULE_ENTRY_STR "osgppuInitModule"

namespace osgPPU
{
    //! Apply some loaded module on the input texture to compute the output
    /**
    * UnitInOutModule does load a module from a dynamic library which is capable
    * of doing processing operations on the input data. Such a module could be for 
    * example a cuda processing module which process the input by cuda.
    **/
    class OSGPPU_EXPORT UnitInOutModule : public UnitInOut
    {
        public:
            META_Node(osgPPU, UnitInOutModule);
        
            UnitInOutModule();
            UnitInOutModule(const UnitInOutModule&, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            
            //! Release it and used memory
            virtual ~UnitInOutModule();

            /**
            * Interface class of a module which can be used with this unit to process the input 
            * data.
            **/
            class Module : public osg::Referenced
            {
                public:
                    //! Default constructor of the module
                    Module(UnitInOutModule* parent) : _parent(parent) {}

                    virtual ~Module(){}

                    /**
                    * Let the module initiaize. Overwrite the method with proper initialization routines.
                    * @return If false is given back the module will not be used and will be unloaded.
                    **/
                    virtual bool init() {return true;}

                    //! This method will be called whenever the rendering has began. (@see noticeBeginRendering())
                    virtual bool beginAndProcess() { return false; }

                    //! End rendering (@see noticeFinishRendering())
                    virtual void end() { }

                protected:
                    UnitInOutModule* _parent;

            };

            typedef bool (*OSGPPU_MODULE_ENTRY)(UnitInOutModule*);

            /**
            * Specify the file name of a dynamic libray containg the module.
            * A method "osgppuInitModule" has to be present in the library.
            * To the method pointer of this unit will be passed to let it
            * setup himself properly.
            * @return true if loading was successful
            **/
            virtual bool loadModule(const std::string& moduleFile);

            /**
            * Set module which will be used to process the input data.
            **/
            virtual void setModule(Module* module);
            
            //! Remove currently used module.
            virtual void removeModule();

            //! Get last loaded module file name
            const std::string& getModuleFile() const { return _moduleFile; }

            //! Get currently loaded module
            Module* getModule() { return _module.get(); }
            const Module* getModule() const { return _module.get(); }

        protected:

            //! Start cuda kernel running over the input textures
            virtual bool noticeBeginRendering(osg::RenderInfo &, const osg::Drawable* );

            //! Stop cuda kernel execution and write results to the output textures
            virtual void noticeFinishRendering(osg::RenderInfo &, const osg::Drawable* );

            bool  _moduleDirty;
            osg::ref_ptr<Module> _module;
            osg::ref_ptr<osgDB::DynamicLibrary> _moduleLib;
            std::string _moduleFile;
    };

};

#endif
