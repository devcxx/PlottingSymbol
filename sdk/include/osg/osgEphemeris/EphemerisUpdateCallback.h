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


#ifndef OSG_EPHEMERIS_UPDATE_CALLBACK_DEF
#define OSG_EPHEMERIS_UPDATE_CALLBACK_DEF

#include <map>
#include <string>

#include <osg/Referenced>
#include <osg/ref_ptr>

#include <osgEphemeris/Export.h>
#include <osgEphemeris/EphemerisData.h>

namespace osgEphemeris {

/**\class EphemerisUpdateCallback
   \brief A callback pure virtual super class for updating EphemerisData at run-time.
   */

class OSGEPHEMERIS_EXPORT EphemerisUpdateCallback : public virtual osg::Referenced
{
    public:
        /** Default constructor.  
          \param name - A string used for registering with the callback registry.
          */
        EphemerisUpdateCallback(const std::string &name):_name(name) {}
        /**
          Pure virtual functor.  Derived classes will implement this 
          operator and update the EphemerisData structure passed in 
          with ephemeris
          */
        virtual void operator()(EphemerisData *ephemeris) = 0;

        /**
          Return the name of the callback
          */
        const std::string &getName() const { return _name; }

    protected:
        virtual ~EphemerisUpdateCallback(){}

        std::string _name;

};

/**\class EphemerisUpdateCallbackRegistry
   \brief Used internally.  A singleton callback registry for loading callbacks from the NodeKit

   */

class OSGEPHEMERIS_EXPORT EphemerisUpdateCallbackRegistry
{
    public:
        /** 
          Default Constructor
         */
        EphemerisUpdateCallbackRegistry();

        /** 
          Return the singleton instance EphemerisUpdateCallbackRegistry
          */

        static EphemerisUpdateCallbackRegistry *instance();
        /**
          Register a EphemerisUpdateCallback callback.  The callback's 
          name is used index it in an internal list of callbacks.
          */
        void registerUpdateCallback( EphemerisUpdateCallback * callback );

        /**
          Get a registered Callback by inquiring by its name
          */
        EphemerisUpdateCallback *getUpdateCallback( const std::string &name );

    protected:

        std::map<std::string, osg::ref_ptr<EphemerisUpdateCallback> > _callbacks;
};

/** \class EphemerisUpdateCallbackProxy
    \brief A template class intended for EphemerisUpdateCallback registration
           upon shared object load.

           */

template <class T>
class EphemerisUpdateCallbackProxy
{
    public:
        EphemerisUpdateCallbackProxy()
        {
            EphemerisUpdateCallbackRegistry::instance()->registerUpdateCallback(new T);
        }
};

}

#endif
