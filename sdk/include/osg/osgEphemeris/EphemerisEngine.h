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

#ifndef OSGEPHEMERIS_EPHEMERIS_ENGINE_DEF
#define OSGEPHEMERIS_EPHEMERIS_ENGINE_DEF

#include <osg/Referenced>
#include <osg/ref_ptr>

#include <osgEphemeris/CelestialBodies.h>

#include <osgEphemeris/Export.h>
#include <osgEphemeris/EphemerisData.h>
#include <osgEphemeris/DateTime.h>

namespace osgEphemeris {

    /**\class EphemerisEngine
       \brief A class containing computational routines for processing heavely body position
              from latitude, longitude, altitude, date and time.
      */

class OSGEPHEMERIS_EXPORT EphemerisEngine: public osg::Referenced
{
    public:

        /**
          Constructor
          */
        EphemerisEngine( EphemerisData *ephemerisData=0L );

        /**
          Set the latitude in degrees
          */
        void setLatitude( double latitude);
        /**
          Set the longitude in degrees.
          */
        void setLongitude( double longitude );
        /**
          Set both the latitude and longitude in degrees.
          */
        void setLatitudeLongitude( double latitude, double longitude );
        /**
          Set the latitude, longitude in degrees and the altitude in meters.
          */
        void setLatitudeLongitudeAltitude( double latitude, double longitude, double altitude );
        /**
          Set the date and time with a DateTime structure.  The parameter passed 
          is copied to the internal DateTime structure.
          */
        void setDateTime( const DateTime &dateTime );
        /**
          Sets the internal DateTime to NOW
        */
        void setDateTime(); 

        /**
          Update heavenly body positions.
          \param updateTime - A boolean which defaults to true.  If true, the current
                              date and time are set by the computer's clock.  If false
                              date and time are not updated.
         */
        void update( bool updateTime=true );
        /**
          Update heavenly body positions.
          \param ephemerisData - A pointer to a specific EphemerisData class
          \param updateTime - A boolean which defaults to true.  If true, the current
                              date and time are set by the computer's clock.  If false
                              date and time are not updated.
          
         */
        void update(EphemerisData *ephemerisData, bool updateTime=true);

        /**
          Compute the local sidereal time. Public so that applications can use
          osgEphemeris as a compute-only engine.
          \param mjd - Mean Julian date.
          \param lng - Negated longitude in degrees.
         */
        static double getLocalSiderealTimePrecise( double mjd, double longitude );


    protected:
        ~EphemerisEngine() {}

        EphemerisData *_ephemerisData;
        osg::ref_ptr<osgEphemeris::Sun >_sun;
        osg::ref_ptr<Moon>      _moon;
        osg::ref_ptr<Mercury>   _mercury;
        osg::ref_ptr<Venus>     _venus;
        osg::ref_ptr<Mars>      _mars;
        osg::ref_ptr<Jupiter>   _jupiter;
        osg::ref_ptr<Saturn>    _saturn;
        osg::ref_ptr<Uranus>    _uranus;
        osg::ref_ptr<Neptune>   _neptune;

        void _updateData( const EphemerisData &, const CelestialBody &, double, CelestialBodyData &);

        static void _getLsnRsn( double mjd, double &lsn, double &rsn);
        static void _getAnomaly( double ma, double s, double &nu, double &ea);
        static void _RADecElevToAzimAlt( 
                double rightAscension,
                double declination,
                double latitude,
                double localSiderealTime,
                double elevation, // In meters above sea level
                double rsn,
                double &azim,
                double &alt );
        static void _calcParallax ( 
                    double tha, double tdec,    // True right ascension and declination
                    double phi, double ht,      // geographical latitude, height abouve sealevel
                    double ehp,                 // Equatorial horizontal parallax
                    double &aha, double &adec); // output: aparent right ascencion and declination
        static void _aaha_aux (double lat, double x, double y, double *p, double *q);
};


}
#endif
