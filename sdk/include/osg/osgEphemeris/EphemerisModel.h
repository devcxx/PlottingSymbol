#include <assert.h>
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

#ifndef OSG_EPHEMERIS_EPHEMERIS_MODEL_DEF
#define OSG_EPHEMERIS_EPHEMERIS_MODEL_DEF

#include <osg/Group>
#include <osg/Transform>
#include <osgUtil/CullVisitor>
#include <osg/ClipPlane>
#include <osg/ClipNode>
#include <osg/LightSource>

#include <osgEphemeris/Export.h>
#include <osgEphemeris/SkyDome.h>
#include <osgEphemeris/GroundPlane.h>
#include <osgEphemeris/MoonModel.h>
#include <osgEphemeris/StarField.h>
#include <osgEphemeris/EphemerisData.h>
#include <osgEphemeris/EphemerisEngine.h>
#include <osgEphemeris/DateTime.h>
#include <osgEphemeris/Planets.h>
#include <osgEphemeris/EphemerisUpdateCallback.h>

namespace osgEphemeris {

/**
  \class EphemerisModel
  \brief The highest level class in the osgEphemeris library allowing for
         the setting of parameters which control the placement of heavenly
         bodies, namely the Sun, Moon, Stars and Planets, in the virtual scene.

         EphemersModel is the programming interface to set the parameters
         for controlling the placement of heavenly bodies, namely the 
         Sun, the Moon, Stars and Planets in the virtual scene.  

         EphemerisModel initializes a SkyDome which controls the 
         sky color and provides a surface for projection of the texture of
         the Sun.  It also retains a pointer to EphemerisData, which
         contains the actual parameters for computing the positions of
         the heavenly bodies, namely latitude, longitude, altitude, date
         and time.

         EphemerisModel by default uses an EphemerisEngine to process
         the information about heavenly body position.  

         EphemerisModel also defines a light, for which it controls the
         position and colors, representing the light from the Sun.  

         EphermerisModel is intended to be a ground bound or near ground
         bound view presentation of heavenly bodies and not intended for
         space-based observation of heavenly bodies.

         EphemerisModel derives from osg::Group and contains an internal 
         traverse() method where it does internal updates at run-time.
         */

class OSGEPHEMERIS_EXPORT EphemerisModel : public osg::Group
{
    public:
        enum Members {
            SUN_LIGHT_SOURCE   = 0x01,
            MOON_LIGHT_SOURCE  = 0x02,
            SKY_DOME           = 0x04,
            GROUND_PLANE       = 0x08,
            MOON               = 0x10,
            PLANETS            = 0x20,
            STAR_FIELD         = 0x40,
            ALL_MEMBERS        = SUN_LIGHT_SOURCE |
                                 MOON_LIGHT_SOURCE |
                                 SKY_DOME |
                                 GROUND_PLANE | 
                                 MOON |
                                 PLANETS | STAR_FIELD,

            DEFAULT_MEMBERS     = SUN_LIGHT_SOURCE |
                                 SKY_DOME |
                                 MOON |
                                 STAR_FIELD
        };

        /** The default constructor */
        EphemerisModel();

        /** The copy contructor */
        EphemerisModel(const EphemerisModel& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

        META_Node(osgEphemerisModel, EphemerisModel);

        /** 
            This sets the source pointer to the Ephemeris data.  By default, EphemerisModel
            will initialize the internal EphemerisData pointer by attaching to the shared
            memory mapped file defined by EphemerisData::getDefaultShmemFile().
            This call may be made if the user intends to use a different EphemerisData source
          */
        void setEphemerisData( EphemerisData *data );
        EphemerisData *getEphemerisData();

        /** 
            Control the members of the Ephemeris Model, which are, in their entirety,
            The sunlight source, a sky dome, a ground plane, the moon, the planets,
            the stars (in a star field).  By default, the sunlight source, skydome,
            moon and starfield are included, excluding the ground plane and planets.
            
            The bitmap may be set by setting any of the bits defined in 
            
                SUN_LIGHT_SOURCE
                MOON_LIGHT_SOURCE
                SKY_DOME
                GROUND_PLANE
                MOON
                PLANETS
                STAR_FIELD
           
               ALL_MEMBERS combines all of the above.
            
               DEFAULT_MEMBERS combines SUN_LIGHT_SOURCE | SKY_DOME | MOON | STAR_FIELD.
           
          */
        void setMembers( unsigned int bit_map_of_members );

          /** Returns as an integer the bitmask representing the set of members currently
            * configured.
            */
        unsigned int getMembers() const;

		/** RSC modifications for sensor simulation.
		  *
		  * Added so that we could get the information from the sky dome to implement
		  * our own fog and overwrite with our own values.
		  */
		SkyDome* getSkyDome() { return _skyDome.get(); }

		/** Added so that we could get the location of the moon in order to render a 
		  * moon halo on top of it
		  */
		osg::MatrixTransform* getMoonTransform() { return _moonTx.get(); }

        /** 
          Set whether the EphemerisModel should auto update its date and time
          using the computer's clock.
          \param flag If True, EphemerisModel will set the heavenely bodies' 
                      position according the current date and time as set
                      by the computer's clock.  If False, date and  time will
                      not be updated internally.  Defaults to True.

          */

        void setAutoDateTime( bool flag);
        /** 
          Return whether AutoDateTime is set.  Returns True if set, False if not
          */
        bool getAutoDateTime() const;

        /**
          Set whether the EphemerisModel should use the internal EphemerisEngine.
          \param flag If True, the heavenly body positions will be updated by 
                      the internal EphemerisEngine.  If False, they will not.
                      Defaults to True.

                      This is useful if the user wants to run an external EphemersEngine
                      on a separate processor, for example.
        */
        void setUseEphemerisEngine(bool flag);

        /**
          Return whether the use of the internal EphemerisEngine is set. 
          */
        bool getUseEphemerisEngine() const;

        /**
          Set the size of the SkyDomeRadius.   
          \param radius_in_meters The desired radius of the SkyDome in meters.  Note
                                  that internally, EphemerisModel uses a scaling factor
                                  from the default SkyDome radius size of "distance from
                                  the earth to the moon", in meters.  Therefore, the 
                                  radius must be specified in meters.

                                  (I know, this sucks a little)
                                  */
        void setSkyDomeRadius( double radius_in_meters );
        /**
           Return the size in meters of the SkyDomeRadius
          */
        double getSkyDomeRadius() const;

        /**
          Set the position of the center of the skydome in 3 space.  
          \param center An osg::Vec3 containing the x,y,z position in 3 space of the
                        SkyDome center.  Note that this should be used even when
                        setMoveWithEyePoint() is set to true because of datbase size
                        initialization regardless of run-time updates.
        */
        void setSkyDomeCenter( osg::Vec3 center );
        /**
          Returns an osg::Vec3 of the position of the center of the SkyDomeCenter.
          It should be noted that this is a static parameter that was set at initialization
          time and does not return the actual center of the skydome if setMoveWithEyePoint
          is set to true.
          */
        osg::Vec3 getSkyDomeCenter();

        /**
          Set which OpenGL light number should be used for the Sun light source.
          */
        void setSunLightNum(unsigned int num);
        /**
          Return which OpenGL light number is being used for the Sun light source.
          */
        unsigned int getSunLightNum() const;
        
        
		/** RSC modifications for sensor simulation.
		  */
      osg::Vec3d getSunPosition();
		osg::Vec4 getSunLightAmbient();
		osg::Vec4 getSunLightDiffuse();

        /**
          Set which OpenGL light number should be used for the Moon light source.
          */
        void setMoonLightNum(unsigned int num);
        /**
          Return which OpenGL light number is being used for the Moon light source.
          */
        unsigned int getMoonLightNum() const;
        
        
		/** RSC modifications for sensor simulation.
		  */
		osg::Vec4 getMoonLightAmbient();
		osg::Vec4 getMoonLightDiffuse();
		

        /**
          Set whether EphemerisModel should move the SkyDome and all components to be
          continually centered around the eyepoint.  Default is True.
          */
        void setMoveWithEyePoint(bool flag);
        /**
          Return whether MoveWithEyePoint is set.
          */
        bool getMoveWithEyePoint() const;

        /**
          Set the current eyepoint latitude in degrees.
          */
        void setLatitude( double latitude );
        /**
          Get the current eyepoint latitude in degrees.
          */
        double getLatitude() const;

        /**
          Set the current eyepoint longitude in degrees.
          */
        void setLongitude( double longitude );
        /**
          Get the current eyepoint longitude in degrees.
          */
        double getLongitude() const;

        /**
          Set the turbidity of the atmosphere.
          */
        void setTurbidity( float turbidity );
        /**
          Get the turbidity of the atmosphere.
          */
        float getTurbidity() const;

        /**
          Set both the latitude and longitude of the current eyepoint
          */
        void setLatitudeLongitude( double latitude, double longitude );
        /**
          Get both the latitude and longitude of the current eyepoint
          */
        void getLatitudeLongitude( double &latitude, double &longitude ) const;

        /**
          Set the latitude, longitude, and altitude of the current eyepoint
          */
        void setLatitudeLongitudeAltitude( double latitude, double longitude, double altitude );
        /**
          Get the latitude, longitude, and altitude of the current eyepoint
          */
        void getLatitudeLongitudeAltitude( double &latitude, double &longitude, double &altitude ) const;

        /** 
          Set the current date and time by passing a DateTime struct
          */
        void setDateTime( const DateTime &dateTime );
        /**
          Get the current date and time setting in a DateTime struct
          */
        DateTime getDateTime() const;

        /**
          Convenience function to get a pointer to the Sun Light Source.
          */
        osg::LightSource* getSunLightSource(){ return _sunLightSource.get(); }

        /**
          Convenience function to get a pointer to the Moon Light Source.
          */
        osg::LightSource* getMoonLightSource(){ return _moonLightSource.get(); }

        /**
          Set the Ephemeris Update Callback.  
          \param updateCallback is a pointer to an osgEphemeris::EphmerisUpdateCallback
                 derived class.  This callback's () operator will be called during 
                 run-time and passed a pointer to the EphemerisModel's EphemerisData,
                 to provide a mechanism to dynamically update the EphemerisData.
         */

        void setEphemerisUpdateCallback( EphemerisUpdateCallback *updateCallback );
        /**
          Return a pointer to the current EphemerisUpdateCallback
          */
        const EphemerisUpdateCallback *getEphemerisUpdateCallback() const;

        void setSkyDomeUseSouthernHemisphere( bool flag ) { _skyDomeUseSouthernHemisphere = flag; }
        bool getSkyDomeUseSouthernHemisphere() { return _skyDomeUseSouthernHemisphere; }

        void setSkyDomeMirrorSouthernHemisphere( bool flag ) { _skyDomeMirrorSouthernHemisphere = flag; }
        bool getSkyDomeMirrorSouthernHemisphere() { return _skyDomeMirrorSouthernHemisphere; }

        /**
          Return a pointer to the GroundPlane
        */
        GroundPlane* getGroundPlane();

        /**
          * Effect an update.  Used internally by the internal UpdateCallback
          */
        virtual void update();

        /**
          Used internally
          */
        virtual void traverse(osg::NodeVisitor&nv);

        /**
          * User requirement to provide a "fudge" factor to the size of the sun -
          * correct size is 0.53 degrees of visual field of view, but user often
          * want to exaggerate this.  You may set a scale factor for the sun here
          */
        void setSunFudgeScale( double scale ); 
        void setMoonFudgeScale( double scale );


    protected:

        bool _inited;
        bool _init();

        // Override one or many of these methods to customize how the 
        // EphemerisModel creates its various members. A derived class could
        // for example create an instance of a derived SkyDome class in 
        // createSkyDome() instead of the osgEphemeris version. The overridden
        // methods should only create the objects, assigning them to the 
        // appropriate member variable below, and set it up appropriately. The
        // _init() method will then add them as child of the EphemerisModel.
        virtual void _createSunLightSource();
        virtual void _createMoonLightSource();
        virtual void _createSkyDome();
        virtual void _createGroundPlane();
        virtual void _createMoon();
        virtual void _createPlanets();
        virtual void _createStarField();

        /**
          Connect clipped or unclipped geometry, depending on if reflections are needed
          */
        void _makeConnections();
        unsigned int _members;
        double _scale;
        osg::Vec3 _center;
        bool _autoDateTime;
        unsigned int _sunLightNum;
        unsigned int _moonLightNum;
        bool _skyDomeUseSouthernHemisphere;
        bool _skyDomeMirrorSouthernHemisphere;

        class UpdateCallback  : public osg::NodeCallback
        {
            public:
                UpdateCallback( EphemerisModel &ephemerisModel ):
                    _ephemerisModel(ephemerisModel) {}

                virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
                {
                    _ephemerisModel.update();
                    traverse(node,nv);
                }

            protected:
                EphemerisModel &_ephemerisModel;
        };

        class MoveWithEyePointTransform : public osg::Transform
        {
            public:
                MoveWithEyePointTransform():_enabled(true) {}

                void setCenter( osg::Vec3 center ) { _center = center; }
                void enable()  { _enabled = true; }
                void disable() { _enabled = false; }
                bool isEnabled() const { return _enabled; }

                virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
                {
                    if( _enabled )
                    {
                        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
                        if (cv)
                        {
                            osg::Vec3 eyePointLocal = cv->getEyeLocal();
                            matrix.preMult(osg::Matrix::translate(
                                        osg::Vec3( eyePointLocal.x(),eyePointLocal.y(),eyePointLocal.z()) - _center));
                        }
                    }
                    return true;
                }
            protected:
                bool _enabled;
                osg::Vec3 _center;
        };

        void _updateSun();
        void _updateMoon();
        void _updateStars();

        osg::ref_ptr<osg::LightSource> _sunLightSource;
		  osg::ref_ptr<osg::LightSource> _moonLightSource;
        osg::ref_ptr<MoveWithEyePointTransform> _ttx;
        osg::ref_ptr<osg::MatrixTransform> _skyTx;
        osg::ref_ptr<SkyDome> _skyDome;
        osg::ref_ptr<GroundPlane> _groundPlane;
        osg::ref_ptr<osg::MatrixTransform> _moonTx;

        osg::ref_ptr<Planets> _planets;

        osg::ref_ptr<MoonModel> _moon;
        osg::ref_ptr<osg::MatrixTransform> _starFieldTx;
        osg::ref_ptr<StarField> _starField;

        osg::ref_ptr<osg::ClipPlane> _clipPlaneTop;
        osg::ref_ptr<osg::ClipNode> _clipNodeTop;
        osg::ref_ptr<osg::ClipPlane> _clipPlaneBottom;
        osg::ref_ptr<osg::ClipNode> _clipNodeBottom;
        osg::ref_ptr<osg::MatrixTransform> _clipReverseTx;

        osg::ref_ptr<osg::Group> _memberGroup;

        osg::Vec3d _sunVec;

        EphemerisData *_ephemerisData;
        osg::ref_ptr<EphemerisEngine> _ephemerisEngine;

        osg::ref_ptr<EphemerisUpdateCallback> _ephemerisUpdateCallback;

        double _sunFudgeScale;
        double _moonFudgeScale;

};


} // namespace
#endif
