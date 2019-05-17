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

#ifndef OSG_EPHEMERIS_MOON_MODEL_DEF
#define OSG_EPHEMERIS_MOON_MODEL_DEF

#include <string>
#include <osgEphemeris/Export.h>
#include <osgEphemeris/Sphere.h>

namespace osgEphemeris {

    /** \class MoonModel
        \brief The model of the Moon.
      */

class OSGEPHEMERIS_EXPORT MoonModel : public Sphere
{
    public:
        /**
          Default constructor
          */
        MoonModel();

        /**
          Return the size of the moon's radius
          */
        static double getMoonRadius() { return _moonRadius; }
        /**
          Set the position of the Sun with respect to the view.  This position
          is used to determine the lighting direction parameters, resulting in
          the correct representation of the phases of the moon.
          */
        void setSunPosition( osg::Vec3 sun );

    protected:

        static const double _moonRadius;
        static std::string _vertexShaderProgram;
        static std::string _fragmentShaderProgram;

        int _baseTextureUnit;
        int _bumpTextureUnit;

        osg::ref_ptr<osg::Uniform> _light;

        static unsigned int  _moonImageLoLodWidth;
        static unsigned int _moonImageLoLodHeight;
        static unsigned int  _moonImageInternalTextureFormat;
        static unsigned int _moonImagePixelFormat;
        static unsigned char _moonImageLoLodData[];
        static unsigned int  _moonImageHiLodWidth;
        static unsigned int _moonImageHiLodHeight;
        static unsigned char _moonImageHiLodData[];

        static unsigned int  _moonNormalImageLoLodWidth;
        static unsigned int _moonNormalImageLoLodHeight;
        static unsigned int  _moonNormalImageInternalTextureFormat;
        static unsigned int _moonNormalImagePixelFormat;
        static unsigned char _moonNormalImageLoLodData[];
        static unsigned int  _moonNormalImageHiLodWidth;
        static unsigned int _moonNormalImageHiLodHeight;
        static unsigned char _moonNormalImageHiLodData[];

        void _buildStateSet();
};

}

#endif
