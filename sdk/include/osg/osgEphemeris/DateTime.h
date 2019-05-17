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


#ifndef OSG_EPHEMERIS_DATE_TIME_DEF
#define OSG_EPHEMERIS_DATE_TIME_DEF

#include <iostream>
#include <time.h>
#include <string>
#include <osg/Referenced>
#include <osgEphemeris/Export.h>
#include <osgEphemeris/IntTypes.h>

namespace osgEphemeris {

/**\class DateTime
   \brief Data and methods to store, query and set the current date and time.
   */

class OSGEPHEMERIS_EXPORT DateTime 
{
    public:
        /** Default Constructor.  Initializes it to now.*/
        DateTime();

        /** Copy Constructor */
        DateTime( const DateTime &dt );

        /** Constructor with parameters 
          \param year  - Actual year, (not since 1900), e.g. 2006
          \param month - Months numbered from 1 (January) to 12 (December)
          \param day   - Day of the month ranges 1 to 31 
          \param hour  - Hour of the day, ranges 0 - 23
          \param minute - minute of the hour, ranges 0 - 59
          \param second - second of the minute, ranges 0 - 59
         */
        DateTime( 
             uint32_t year,          // Actual year.  e.g. 2006
             uint32_t month,         // Month January = 1, December = 12
             uint32_t day,           // 1 - 31
             uint32_t hour=0,        // 0 - 23
             uint32_t minute = 0,    // 0 - 59
             uint32_t second = 0 );  // 0 - 59


        /** Constructor with parameter
            \param struct tm - A filled in struct tm (time.h) structure.
            */
        DateTime( const struct tm & );

        /**
          Set the current date and time to NOW.
          */
        void now();

        /**
          get the Modified Julian Date based on GMT, from the current date and time.
          */
        double      getModifiedJulianDate() const; 

        /**
          Get the Greenwhich Mean Time from the current date and time.
          */
        DateTime    getGMT() const;

        /**
          Set the current month
          \param year - Actual year, e.g. 2006.
          */
        void setYear( uint32_t year);
        /**
          Get the current year.
          */
        uint32_t getYear() const;

        /** 
          Set the month.
          \param month - An integer ranging from 1= January to 12 = December.
          */
        void setMonth( uint32_t month );
        /**
          Get the current month.
          */
        uint32_t getMonth() const;
        /** 
          Get a std::string representing the name of the current month.
          */
        std::string getMonthString() const;
        /**
          Static method to get the name of a month, based on a parameter.
          \param month - An integer ranging from 1= January to 12 = December.
          */
        static std::string getMonthString(uint32_t month);

        /**
          Set the day of the month. 
          \param day - An integer ranging from 1 to 31.
          */
        void setDayOfMonth( uint32_t day);

        /**
          Get the day of the month
          */
        uint32_t getDayOfMonth() const;

        /**
          Get the day of the year, range 1 - 366 based on the current date.
          */
        uint32_t getDayOfYear() const;
        /**
          Get the day of the week range 1 - 7 base on the current date.
          */
        uint32_t getDayOfWeek() const;
        /** 
          Get an std::string with the name of the day of the week based on the current date.
          */
        std::string getDayOfWeekString() const;
        /**
          Static method to get the name of day of the week with a parameter.
          \param weekday - An integer range 1 - 7.
          */
        static std::string getDayOfWeekString( uint32_t weekday );

        /**
          Set the current hour of the day
          \param hr - An integer ranging 0 - 23.
          */
        void setHour( uint32_t hr );
        /**
          Get the current hour.  Returns an integer ranging 0 - 23.
          */
        uint32_t getHour() const;

        /**
          Set the current minute of the hour
          \param min - An integer ranging 0 - 59.
          */
        void setMinute( uint32_t  min);
        /**
          Get the current minute of the hour.  Returns an integer ranging 0 - 59.
          */
        uint32_t getMinute() const;

        /**
          Set the current second of the minute
          \param sec - An integer ranging 0 - 59.
          */
        void setSecond( uint32_t sec );
        /**
          Get the current second of the minute.  Returns an integer ranging 0 - 59.
          */
        uint32_t getSecond() const;

        /**
          Return a boolean value indicating whether the currently set date falls during 
          daylight savings time.
          */
        bool isDaylightSavingsTime() const;

        void setTimeZoneOffset( bool useSystemTimeZone, int32_t hours );
        int32_t getTimeZoneOffset() const;

    protected:
        struct tm _tm;
        int32_t _tzoff;
        static const char *weekDayNames[7];
        static const char *monthNames[12];
};


inline std::ostream& operator << (std::ostream& output, const DateTime& dt)
{
    output << dt.getDayOfWeekString() << " " << dt.getMonthString() << " " <<
              dt.getDayOfMonth() << " " <<
              dt.getHour() << ":" << dt.getMinute() << ":" << dt.getSecond() << 
              " " << dt.getYear(); 

    return output;     // to enable cascading
}


}

#endif
