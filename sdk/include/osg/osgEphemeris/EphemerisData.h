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


#ifndef EPHEMERIS_DATA_DEF
#define EPHEMERIS_DATA_DEF

#include <osgEphemeris/Export.h>
#include <osgEphemeris/Shmem.h>
#include <osgEphemeris/DateTime.h>

namespace osgEphemeris {

namespace CelestialBodyNames {

    /**\enum CelestialBodyName 
      */
enum CelestialBodyName {
    Sun     = 0,
    Moon,
    Mercury,
    Venus,
    //Earth, -  Don't uncomment Comment left as a placeholder. (Ya can't see earth from earth).
    Mars,
    Jupiter,
    Saturn,
    Uranus,
    Neptune,
    Pluto
};

}

/**\class CelestialBodyData
   \brief A container to keep information about the current position of a celestial body.

   */

struct OSGEPHEMERIS_EXPORT CelestialBodyData
{
    /*!
      The name of the Celestial body
      */
    char name[64];
    /*!
      The current right ascension of the celestial body.  Right ascension is the
      angle of rotation from directly north and rotating clockwise that the position
      of the celestial body projects to on the current tangent plane.
      */
    double rightAscension;
    /*!
      The current declination of the celestial body.  Declination refers to the
      angle above the horizon the celestial body is.
      */
    double declination;
    /*!
      The magnitude of this celestial body.  Used for determining brightness in the
      sky
      */
    double magnitude;
    /*!
      Azimuth of the position of the celestial body.  This is computed and filled in 
      by the EphemerisEngine at run time from the rightAscension and Declination, using
      the current sidereal time.
      */
    double azimuth;
    /*!
      Altitude of the position of the celestial body.  This is computed and filled in 
      by the EphemerisEngine at run time from the rightAscension and Declination, using
      the current sidereal time.
      */
    double alt;
};

/** \class EphemerisData
    \brief A container for the parameters used by the EphemerisEngine to compute
           heavenly body positions and to store those positions.

          EphemerisData is a container for the parameters used by EphemerisEngine
          to compute heavenly body positions and to store those positions after
          computation.  
  */


struct OSGEPHEMERIS_EXPORT EphemerisData //: public Shmem
{
    /*! 
      Latitude of view point in degrees 
     */
    double latitude;
    /*! 
      Longitude of view point in degrees 
     */
    double longitude;
    /*! 
      Altitude of view point in meters 
     */
    double altitude;

    /*! 
      Turbidity of atmosphere near the ground.
      Turbidity = ( scattering(haze) + scattering(molecules) ) / scattering(molecules)
      Reasonable values are {1.0, 60.0}
     */
    float turbidity;

    /*! 
      Date and Time for which to compute the ephemeris
     */
    DateTime dateTime;

    /*!
      Modified Julian Date, updated from the current Date and 
      Time in the dateTime member by the EphemerisEngine
      */
    double modifiedJulianDate;
    /*!
      Local Sidereal Time, updated by the EphemerisEngine from the current 
      latitude, longitude, date and time
      */
    double localSiderealTime;

    /*!
       An array of 12 CelestialBodyData structs.  One for the Sun, one for the
       moon and 8 for planets (excluding Earth). (2 extra ?)
      */ 
    CelestialBodyData data[12];

    /**
      Return a string containing the name of the file on the file system used
      for memory mapping into shared memory space.
      */
    static std::string getDefaultShmemFileName() { return _defaultShmemFileName; }

    protected:
        static const std::string _defaultShmemFileName; 
};

}

#endif
