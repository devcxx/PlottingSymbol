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

#ifndef OSG_EPHEMERIS_SPHERE_DEF
#define OSG_EPHEMERIS_SPHERE_DEF

#include <osg/Geode>
#include <osg/LOD>

#include <osgEphemeris/Export.h>

namespace osgEphemeris {

/** \class Sphere
    \brief A geometric sphere containing a northern and southern hemisphere.

    */
           

class OSGEPHEMERIS_EXPORT Sphere : public osg::Group
{
    public:
        enum Orientation
        {
            InnerOrientation,
            OuterOrientation,
        };

        enum TesselationResolution 
        {
            TessHigh,
            TessNormal,
            TessLow
        };

        enum Hemisphere {
            NorthernHemisphere = 1,
            SouthernHemisphere = 2 ,
            BothHemispheres = 3
        };

        bool _skyTexCoords;

        Sphere( double radius=_defaultRadius,
                TesselationResolution tr=TessNormal,
                Orientation orientation=OuterOrientation,
                Hemisphere hemisphere=BothHemispheres,
                bool _skyTexCoords=false
              );


        osg::Geode *getNorthernHemisphere() { return _northernHemisphere.get(); }
        osg::Geode *getSouthernHemisphere() { return _southernHemisphere.get(); }
        static double getDefaultRadius();

    protected:

        osg::ref_ptr<osg::Geode> _northernHemisphere;
        osg::ref_ptr<osg::Geode> _southernHemisphere;

        static const double _defaultRadius;
        
        osg::Vec2 makeTexCoord(osg::Vec3 &normal, unsigned int sector);
};


/** \class SphereLOD
    \brief A container class for three Sphere's representing progressional levels of detail.

    */

class OSGEPHEMERIS_EXPORT SphereLOD : public osg::LOD
{
    public:
        SphereLOD( double radius=Sphere::getDefaultRadius(), 
                   Sphere::Orientation orientation=Sphere::OuterOrientation,
                   Sphere::Hemisphere hemisphere=Sphere::BothHemispheres );

};

}

#endif

