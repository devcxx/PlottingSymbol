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

#ifndef OSG_EPHEMERIS_STAR_FIELD_DEF
#define OSG_EPHEMERIS_STAR_FIELD_DEF

#include <fstream>
#include <sstream>
#include <vector>

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Uniform>

#include <osgEphemeris/Export.h>

namespace osgEphemeris {

/** \class StarField
    \brief A spherical set of points representing the positions of the stars as 
           seen from a single viewpoint on the surface of the earth.

    StarField contains a set of points set at a constant radius from a center.
    Each point represents the projection of a star in the sky on to a sphere
    with radius as specified in the constructor.  Each point represents the 
    position of the star and is rendered with a brightness that approximates
    the corresponding magnitude as specified in a database of stars.

    The current database of stars come from The Yale Bright Star Catalogue, 
    from the YBS.edb file of XEphem 3.5.2.  

    No attempt is made to position stars in reference to each other over long
    periods of time.  The assumption is made that star positions are representative
    of constellation make up of current times.  Thus, the entire set of points
    is rotated uniformly to position them with approximation to the correct
    positions in the sky given latitude, longitude, date and time.
    */

class OSGEPHEMERIS_EXPORT StarField: public osg::MatrixTransform
{
    public:
        /**
          Constructor.
          \param fileName - The name of the file containing the catalogue of stars
          \param radius   - The radius of the projection sphere 
          */
        StarField( const std::string &fileName="", double radius=_defaultRadius );

        /**
          Get the total number of stars in the Star Field
          */
        unsigned int getNumStars();

        /**
          Set the SunAltitude.  This is used to determine brightness of the 
          stars according to daylight scatter in the atmosphere.  When the sun is 
          below the horizon, stars are visible, when above they are not.
          */
        virtual void setSunAltitude(  double altitude );

    protected:
        struct StarData {
            std::string name;
            double right_ascension;
            double declination;
            double magnitude;

            StarData( std::stringstream &ss );
        };

        std::vector<StarData> _stars;
        double _radius;
        static const double _defaultRadius;
        osg::ref_ptr<osg::Geode> _starGeode;
        osg::ref_ptr<osg::Geode> _starLabelsGeode;

        bool _parseFile( const std::string fileName );
        void _parseStream( std::istream & );
        void _buildGeometry(void);
        void _buildLabels(void);

        static std::string _vertexShaderProgram;
        static std::string _fragmentShaderProgram;
        osg::ref_ptr<osg::Uniform> _starAlpha;
        osg::ref_ptr<osg::Uniform> _pointSize;
        osg::ref_ptr<osg::Uniform> _MVi;;

};

}

#endif
