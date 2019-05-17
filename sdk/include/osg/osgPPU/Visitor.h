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
#ifndef _C_VISITOR__H_
#define _C_VISITOR__H_

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------
#include <osgPPU/Export.h>
#include <osgPPU/Processor.h>
#include <osgPPU/Unit.h>
#include <osg/NodeVisitor>
#include <osg/Group>
#include <osgUtil/CullVisitor>
#include <queue>
#include <list>

namespace osgPPU
{

//------------------------------------------------------------------------------
// Base class for all unit visitors
//------------------------------------------------------------------------------
class OSGPPU_EXPORT UnitVisitor : public osg::NodeVisitor
{
    public:
        META_NodeVisitor("osgPPU" , "UnitVisitor");

        virtual void run(osg::Group* root) { root->traverse(*this); }
        inline  void run(osg::Group& root) { run(&root); }

        static OpenThreads::Mutex s_mutex_changeUnitSubgraph;
};

//------------------------------------------------------------------------------
// Visitor to set update traversed flag to false
//------------------------------------------------------------------------------
class OSGPPU_EXPORT CleanUpdateTraversedVisitor : public UnitVisitor
{
public:

    CleanUpdateTraversedVisitor() : UnitVisitor()
    {
    }

    void apply (osg::Group &node)
    {
        Unit* unit = dynamic_cast<Unit*>(&node);
        if (unit) unit->mbUpdateTraversed = false;
        node.traverse(*this);
    }

    void run (osg::Group* root);

    const char* className() { return "CleanUpdateTraversedVisitor"; }

    static osg::ref_ptr<CleanUpdateTraversedVisitor> sVisitor;

private:
    OpenThreads::Mutex _mutex;
};

//------------------------------------------------------------------------------
// Visitor to set update traversed flag to false
//------------------------------------------------------------------------------
class OSGPPU_EXPORT CleanCullTraversedVisitor : public UnitVisitor
{
public:

    CleanCullTraversedVisitor() : UnitVisitor()
    {
    }

    void apply (osg::Group &node);

    void run (osg::Group* root);

    const char* className() { return "CleanCullTraversedVisitor"; }

    static osg::ref_ptr<CleanCullTraversedVisitor> sVisitor;

private:
    OpenThreads::Mutex _mutex;
};


//------------------------------------------------------------------------------
// Helper visitor to setup maximum number of input attachments
//------------------------------------------------------------------------------
class OSGPPU_EXPORT SetMaximumInputsVisitor : public UnitVisitor
{
public:

    SetMaximumInputsVisitor(unsigned int max) : UnitVisitor()
    {
        mMaxUnitInputIndex = max;
    }

    void apply (osg::Group &node);
    void run (osg::Group* root);

    const char* className() { return "SetMaximumInputsVisitor"; }

private:
    unsigned int mMaxUnitInputIndex;
};


//------------------------------------------------------------------------------
// Visitor to find a certain unit in the unit graph
//------------------------------------------------------------------------------
class OSGPPU_EXPORT FindUnitVisitor : public UnitVisitor
{
public:

    FindUnitVisitor(const std::string& name) : UnitVisitor(),
        _name(name), _result(NULL)
    {
    }

    void apply (osg::Group &node)
    {
        // first check if we have already visited that node, if yes, it might be a loop in the graph, so don't go further
        if (std::find(_visitedNodes.begin(), _visitedNodes.end(), &node) != _visitedNodes.end()) return;
        
        Unit* unit = dynamic_cast<Unit*>(&node);
        if (unit && unit->getName() == _name)
            _result = unit;
        else
        {
            _visitedNodes.push_back(&node);
            node.traverse(*this);
        }
    }

    Unit* getResult() { return _result; }

    const char* className() { return "FindUnitVisitor"; }

private:
    std::string _name;
    Unit* _result;
    std::vector<osg::Group*> _visitedNodes;
};

//------------------------------------------------------------------------------
// Visitor to find and remove certain unit from the unit graph
// all inputs of the unit are placed as inputs to children units
//------------------------------------------------------------------------------
class OSGPPU_EXPORT RemoveUnitVisitor : public UnitVisitor
{
public:

    RemoveUnitVisitor() : UnitVisitor()
    {
    }

    void run (osg::Group* root);

    const char* className() { return "RemoveUnitVisitor"; }
};


//------------------------------------------------------------------------------
// Visitor to optimize unit subgraph
//------------------------------------------------------------------------------
class OSGPPU_EXPORT OptimizeUnitsVisitor : public UnitVisitor
{
public:

    OptimizeUnitsVisitor() : UnitVisitor(),
        _maxUnitInputIndex(0)
    {
    }

    void apply (osg::Group &node);
    void run (osg::Group* root);

    const char* className() { return "OptimizeUnitsVisitor"; }
private:
    unsigned _maxUnitInputIndex;
};

//------------------------------------------------------------------------------
// Visitor to resolve all cycles in the unit graph
// This will add BarrierNodes where they are needed
//------------------------------------------------------------------------------
class OSGPPU_EXPORT ResolveUnitsCyclesVisitor : public UnitVisitor
{
public:

    ResolveUnitsCyclesVisitor() : UnitVisitor()
    {
    }

    void apply (osg::Group &node);
    void run (osg::Group* root);

    const char* className() { return "ResolveUnitsCyclesVisitor"; }
};

//------------------------------------------------------------------------------
// Visitor used to setup all units in a correct order in an appropriate rendering bin
// every unit will also be initialized
//------------------------------------------------------------------------------
class OSGPPU_EXPORT SetupUnitRenderingVisitor : public UnitVisitor
{
public:

    SetupUnitRenderingVisitor(Processor* proc) : UnitVisitor(), _proc(proc)
    {
        _startIndex = proc ? proc->getOrCreateStateSet()->getBinNumber() : 0;
        _binName = proc ? proc->getOrCreateStateSet()->getBinName() : "DefaultPPUBin";
        _initUnits = true;
    }

    void apply (osg::Group &node);
    void run (osg::Group* root);

    void setStartIndex(unsigned ind) { _startIndex = ind; }
    void setInitUnitsWhenFound(bool b) { _initUnits = b; }
    void setBinName(const std::string& name) { _binName = name; }

    const char* className() { return "SetupUnitRenderingVisitor"; }
private:
    typedef std::list<Unit*> UnitSet;
    Processor* _proc;
    UnitSet mUnitSet;
    bool _initUnits;
    unsigned _startIndex;
    std::string _binName;
};


//--------------------------------------------------------------------------
// Helper class to find the processor
//--------------------------------------------------------------------------
class OSGPPU_EXPORT FindProcessorVisitor: public UnitVisitor
{
public:
    FindProcessorVisitor() : UnitVisitor(), _processor(NULL)
    {
        setTraversalMode(osg::NodeVisitor::TRAVERSE_PARENTS);
    }

    void apply(osg::Group& node)
    {
        _processor = dynamic_cast<osgPPU::Processor*>(&node);
        if (_processor == NULL) traverse(node);
    }

    osgPPU::Processor* _processor;

    const char* className() { return "FindProcessorVisitor"; }
};

//------------------------------------------------------------------------------
// Mark every unit in the graph as dirty
//------------------------------------------------------------------------------
class OSGPPU_EXPORT MarkUnitsDirtyVisitor : public UnitVisitor
{
public:

    MarkUnitsDirtyVisitor() : UnitVisitor()
    {
    }

    void apply (osg::Group &node);
    void run (osg::Group* root);

    const char* className() { return "MarkUnitsDirtyVisitor"; }
private:
    OpenThreads::Mutex _mutex;
};

//------------------------------------------------------------------------------
// Remove viewports on units which has -1 as index for viewport reference
//------------------------------------------------------------------------------
class OSGPPU_EXPORT RemoveUnitsViewportsVisitor : public UnitVisitor
{
public:

    RemoveUnitsViewportsVisitor(int index = -1) : UnitVisitor(), _index(index)
    {
    }

    void apply (osg::Group &node);
    void run (osg::Group* root);

    const char* className() { return "RemoveUnitsViewportsVisitor"; }
private:
    OpenThreads::Mutex _mutex;
    int _index;
};

}; // end namespace

#endif
