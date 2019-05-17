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
#ifndef _C_BARRIER__H_
#define _C_BARRIER__H_

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/Export.h>
#include <osgPPU/Unit.h>
#include <osg/NodeVisitor>
#include <osg/Group>

namespace osgPPU
{
    /**
    * Helper class to block cycle traversion throught the nodes
    **/
    class BarrierNode : public osg::Node
    {
        public:
            BarrierNode() : osg::Node() {}
            ~BarrierNode(){}
    
            //! The barrier node do just block the traversion, hence childs are not traversed
            void traverse (osg::NodeVisitor &nv){}
    
            //! The barrier node do just block the traversion, hence childs are not traversed
            void ascend (osg::NodeVisitor &nv){}

            //! Since traversion is blocked, we should allow the computation of bounding sphere    
            inline osg::BoundingSphere  computeBound() const {return osg::BoundingSphere();}

            //! Get the node, which was previously the child
            inline osg::Node* getBlockedChild() { return _child; }
            inline const osg::Node* getBlockedChild() const { return _child; }

            //! Set child which is now blocked
            inline void setBlockedChild(osg::Node* child) { _child = child; }
            
            //! Get the node, which was previously the parent
            inline osg::Group* getBlockedParent() {return _parent;}

            //! Set parent which is now blocked
            inline void setBlockedParent(osg::Group* parent){_parent = parent;}

        private:
            osg::Node* _child;
            osg::Group* _parent;
    };

}; // end namespace

#endif
