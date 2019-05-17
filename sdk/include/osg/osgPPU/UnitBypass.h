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

#ifndef _C_UNIT_BYPASS_H_
#define _C_UNIT_BYPASS_H_


//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/Export.h>
#include <osgPPU/Unit.h>

namespace osgPPU
{
    //! Unit which do simply bypass input to the output
    /**
    * The result of this ppu is similar to Unit::setOutputTextreMap(Unit::getInputTextureMap());
    **/
    class OSGPPU_EXPORT UnitBypass : public Unit {
        public:
            META_Node(osgPPU,UnitBypass);
        
            UnitBypass();
            UnitBypass(const UnitBypass& u, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
            
            virtual ~UnitBypass();
    
        protected:
            virtual void setupInputsFromParents();
            virtual bool noticeBeginRendering (osg::RenderInfo&, const osg::Drawable* ) { return false; }            
            virtual void traverse(osg::NodeVisitor& nv) { return Unit::traverse(nv); }
    };
};

#endif
