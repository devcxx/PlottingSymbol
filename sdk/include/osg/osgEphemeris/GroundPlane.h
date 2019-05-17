/*
 -------------------------------------------------------------------------------
 | osgEphemeris - Copyright (C) 2007  Don Burns                                |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 |                                                                             |
 -------------------------------------------------------------------------------
 */

#ifndef OSG_EPHEMERIS_GROUND_PLANE_DEF
#define OSG_EPHEMERIS_GROUND_PLANE_DEF

#include <map>

#include <osg/Geode>
#include <osg/Geometry>

#include <osgEphemeris/Export.h>
namespace osg
{
    class Texture2D;
    class Image;
    class StateSet;
    class Vec3f;
    class Vec2f;
}

namespace osgEphemeris {

    /** \class GroundPlane
        \brief A geometric model of the ground plane at elevation 0.
        */

class OSGEPHEMERIS_EXPORT GroundPlane: public osg::Geode
{
    public:
        /** 
          Constructor.
          \param radius - the radius of the ground plane.  This should 
                          correspond with the radius of the SkyDome
        */
        GroundPlane( double radius );

        ///Update the base terrain from an image
        ///\param colorTerrainImage The color image of the terrain
        bool UpdateBaseTerrainFromImage(std::string colorTerrainImage);

        ///Set the range for the altitude  
        ///\param minHeight The low end of altitude
        ///\param maxHeight The high end of altitude
        void SetAltitudeRange(float minHeight, float maxHeight); 

        ///Update the texture bindings
        ///\param ss The osg::StateSet to update 
        ///\param vertTexture The texture referenced in the vertex shader
        ///\param fragTexture The texture referenced in the frag shader
        void UpdateTextureUnitsInStateSet( osg::StateSet* ss,
                                           osg::Texture2D* vertTexture);

    protected:
        ///Initialize shaders uniforms statesets and textures
        void _initializeSimpleTerrainGeneratorShader();

        ///\param numCols Number of colums in the terrain grid
        ///\param numRows Number of rows in the terrain grid
        void _updateTerrainGrid(unsigned int numCols, unsigned int numRows);
        std::map<std::string,osg::ref_ptr<osg::StateSet> > m_terrainPrograms;///<Terrain shaders
        osg::ref_ptr<osg::Image> m_baseTerrainImage;///<The color image of the base terrain 
        std::map< std::string, osg::ref_ptr< osg::Texture2D > > m_terrainVertTexture;///<The terrain texture object used in the vertex shader

        double m_radius;///<The radius for the terrain. Corresponds to the SkyDome radius
        osg::ref_ptr<osg::Geometry> m_terrainGrid;///<The geometry for the grid
        osg::Vec3 m_origin;///<The origin (lowerleft corner) of the terrain grid
        osg::Vec3 m_size;///<Total size of the terrain. Creates a 2*size square grid
        osg::Vec3 m_scaleDown;///<Scale down factor for texture coords
        osg::Vec2 m_altitudeRange;///<Range for the altitude
        
};

}

#endif
