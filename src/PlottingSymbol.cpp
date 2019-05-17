/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
* Copyright 2016 Pelican Mapping
* http://osgearth.org
*
* osgEarth is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include <osgViewer/Viewer>
#include <osgEarth/Notify>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>
#include <osgEarth/MapNode>
#include <osgEarth/ThreadingUtils>
#include <osgEarth/Metrics>
#include <iostream>

#include "DrawLineTool.h"
#include "DrawRectangleTool.h"
#include "DrawPolygonTool.h"
#include "DrawCircleTool.h"
#include "GeoStraightArrow.h"
#include "GeoDoubleArrow.h"
#include "GeoDiagonalArrow.h"
#include "GeoGatheringPlace.h"
#include "GeoLune.h"
#include "GeoParallelSearch.h"
#include "GeoSectorSearch.h"

#define LC "[viewer] "

enum ToolType {
    TOOL_CLEAR, // 清除绘制
    TOOL_DRAW_LINE, // 线
    TOOL_DRAW_CIRCLE, // 圈
    TOOL_DRAW_POLYGON, // 多边形
    TOOL_DRAW_RECTANGLE, // 矩形
    TOOL_MEASURE_HEIGHT, // 高度测量
    TOOL_PLACE_MARK, // 点标记
    TOOL_IMAGE_OVERLAY, // 遮盖标记
    TOOL_DRAW_ARROWED_LINE, // 箭头
    TOOL_DRAW_BEZIER_CURVE, // 曲线
    TOOL_RADIAL_LOS,
    TOOL_MEASURE_AREA, // 面积测量
    TOOL_MEASURE_DISTANCE, // 距离测量
    TOOL_MEASURE_ANGLE, // 角度测量
    TOOL_DRAW_STRAIGHTARROW, // 直箭头
    TOOL_DRAW_DOUBLEARROW, // 双箭头
    TOOL_DRAW_DIAGONALARROW, // 斜箭头
    TOOL_DRAW_GATHERINGPLACE, // 聚居区
    TOOL_DRAW_GEOLUNE, // 弓形
    TOOL_DRAW_PARALLELSEARCH, //平行搜寻区
    TOOL_DRAW_SECTORSEARCH, //扇形搜寻区
};

using namespace osgEarth;
using namespace osgEarth::Util;

int
usage(const char* name)
{
    OE_NOTICE 
        << "\nUsage: " << name << " file.earth" << std::endl
        << MapNodeHelper().usage() << std::endl;

    return 0;
}

std::map<int, osg::ref_ptr<osgGA::GUIEventHandler>> g_toolMap;
std::vector<ToolType> g_toolTypes;
int g_currToolIndex = 0;
osg::Group* g_drawGroup = NULL;

// 初始化工具集
void initTools(osgEarth::MapNode* mapNode) {
    g_drawGroup = new osg::Group;
    mapNode->addChild(g_drawGroup);
    g_toolTypes.push_back(TOOL_CLEAR);

    g_toolMap[TOOL_DRAW_LINE] = new DrawLineTool(mapNode, g_drawGroup);
    g_toolTypes.push_back(TOOL_DRAW_LINE);

    g_toolMap[TOOL_DRAW_RECTANGLE] = new DrawRectangleTool(mapNode, g_drawGroup);
    g_toolTypes.push_back(TOOL_DRAW_RECTANGLE);

    g_toolMap[TOOL_DRAW_POLYGON] = new DrawPolygonTool(mapNode, g_drawGroup);
    g_toolTypes.push_back(TOOL_DRAW_POLYGON);

    g_toolMap[TOOL_DRAW_CIRCLE] = new DrawCircleTool(mapNode, g_drawGroup);
    g_toolTypes.push_back(TOOL_DRAW_CIRCLE);

    g_toolMap[TOOL_DRAW_STRAIGHTARROW] = new GeoStraightArrow(mapNode, g_drawGroup);
    g_toolTypes.push_back(TOOL_DRAW_STRAIGHTARROW);
    g_toolMap[TOOL_DRAW_DOUBLEARROW] = new GeoDoubleArrow(mapNode, g_drawGroup);
    g_toolTypes.push_back(TOOL_DRAW_DOUBLEARROW);
    g_toolMap[TOOL_DRAW_DIAGONALARROW] = new GeoDiagonalArrow(mapNode, g_drawGroup);
    g_toolTypes.push_back(TOOL_DRAW_DIAGONALARROW);
    g_toolMap[TOOL_DRAW_GATHERINGPLACE] = new GeoGatheringPlace(mapNode, g_drawGroup);
    g_toolTypes.push_back(TOOL_DRAW_GATHERINGPLACE);
    g_toolMap[TOOL_DRAW_GEOLUNE] = new GeoLune(mapNode, g_drawGroup);
    g_toolTypes.push_back(TOOL_DRAW_GEOLUNE);
    g_toolMap[TOOL_DRAW_PARALLELSEARCH] = new GeoParallelSearch(mapNode, g_drawGroup);
    g_toolTypes.push_back(TOOL_DRAW_PARALLELSEARCH);
    g_toolMap[TOOL_DRAW_SECTORSEARCH] = new GeoSectorSearch(mapNode, g_drawGroup);
    g_toolTypes.push_back(TOOL_DRAW_SECTORSEARCH);
}

// 设置当前激活工具
void setActivateTool(osgViewer::View* view, ToolType type)
{
    for (auto it = g_toolMap.begin(); it != g_toolMap.end(); it++) {
        view->removeEventHandler(it->second);
    }

    auto& fi = g_toolMap.find(type);

    if (fi != g_toolMap.end()) {
        view->addEventHandler(fi->second);
    }

    if (type == TOOL_CLEAR) {
        g_drawGroup->removeChildren(0, g_drawGroup->getNumChildren());
    }
}

// 快捷键处理
class Shortcuts : public osgGA::GUIEventHandler {
public:
    explicit Shortcuts( osgViewer::View* view) : view_(view) {}

    virtual bool handle(const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& aa) {

        if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN) {
            if ((ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_ALT) != 0) {
                switch (ea.getKey()) {
                case osgGA::GUIEventAdapter::KEY_F4:
                    if ((ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_ALT) != 0) {
                        ::exit(0);
                        return true;
                    }
                    break;
                }
            } else {
                switch (ea.getKey()) {

                case osgGA::GUIEventAdapter::KEY_1: // 按1循环工具集
                {
                    if (g_currToolIndex < g_toolMap.size())
                        g_currToolIndex++;
                    else
                        g_currToolIndex = 0;
                    setActivateTool(view_, g_toolTypes[g_currToolIndex]);
                    break;
                }

                }
            }

        }
        return false;

    }

private:
    osgViewer::View* view_;

};

int
main(int argc, char** argv)
{
    osg::ArgumentParser arguments(&argc,argv);

    // help?
    if ( arguments.read("--help") )
        return usage(argv[0]);

    float vfov = -1.0f;
    arguments.read("--vfov", vfov);

    

    // create a viewer:
    osgViewer::Viewer viewer(arguments);

    // Tell the database pager to not modify the unref settings
    viewer.getDatabasePager()->setUnrefImageDataAfterApplyPolicy( true, false );

    // thread-safe initialization of the OSG wrapper manager. Calling this here
    // prevents the "unsupported wrapper" messages from OSG
    osgDB::Registry::instance()->getObjectWrapperManager()->findWrapper("osg::Image");

    // install our default manipulator (do this before calling load)
    viewer.setCameraManipulator( new EarthManipulator(arguments) );

    // disable the small-feature culling
    viewer.getCamera()->setSmallFeatureCullingPixelSize(-1.0f);

    // set a near/far ratio that is smaller than the default. This allows us to get
    // closer to the ground without near clipping. If you need more, use --logdepth
    viewer.getCamera()->setNearFarRatio(0.0001);

    if ( vfov > 0.0 )
    {
        double fov, ar, n, f;
        viewer.getCamera()->getProjectionMatrixAsPerspective(fov, ar, n, f);
        viewer.getCamera()->setProjectionMatrixAsPerspective(vfov, ar, n, f);
    }

    // load an earth file, and support all or our example command-line options
    // and earth file <external> tags    
    osg::Node* node = MapNodeHelper().load(arguments, &viewer);
    if ( node )
    {
        viewer.setSceneData( node );
        viewer.addEventHandler(new Shortcuts(&viewer));
        initTools(MapNode::get(node));
        Metrics::run(viewer);
    }
    else
    {
        return usage(argv[0]);
    }

    return 0;
}
