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

#ifndef OSG_EPHEMERIS_SKY_DOME_DEF
#define OSG_EPHEMERIS_SKY_DOME_DEF

#include <osg/Drawable>
#include <osg/Texture1D>
#include <osg/Texture2D>
#include <osg/TexGen>

#include <osg/MatrixTransform>

#include <osgEphemeris/Export.h>
#include <osgEphemeris/Sphere.h>

namespace osgEphemeris {

    /** \class SkyDome
        \brief A Geometric sphere which defines the color of the sky and serves
               as a surface for projecting the sun texture - Used internally.

      */

class OSGEPHEMERIS_EXPORT SkyDome:  public Sphere
{
    public:
        /**
          Default Constructor
          */
        SkyDome( bool useBothHemispheres=true, bool MirrorInBothHemispheres=true );

        void setSunPos( double azimuth, double altitude );
        void setTurbidity( float t );
        virtual void traverse(osg::NodeVisitor&);
        static double getMeanDistanceToMoon() { return _meanDistanceToMoon; }
        
        void setSunFudgeScale( double sunFudgeScale ){ _sunFudgeScale = sunFudgeScale; }
        double getSunFudgeScale( ){ return _sunFudgeScale; }
		/** RSC modifications for sensor simulation.
		  */
		osg::Image* getSkyImage(void);
		void setSkyImage(osg::Image*);

    protected:
    
		/** RSC modifications for sensor simulation.
		  */
		bool _overrideSkyImage;

        static const double _meanDistanceToMoon;

        double _sunAzimuth;
        double _sunAltitude;
        double _sunFudgeScale;

        unsigned int _skyTextureUnit;
        osg::ref_ptr<osg::Texture2D> _skyTexture;

        unsigned int _sunTextureUnit;
        osg::ref_ptr<osg::Texture2D> _sunTexture;
        osg::ref_ptr<osg::Image> _sunImage;
        osg::ref_ptr<osg::TexGen> _sunTexGenNorth;
        osg::ref_ptr<osg::TexGen> _sunTexGenSouth;

        bool _mirrorInSouthernHemisphere;

        class SectorUpdateCallback: public osg::Drawable::UpdateCallback
        {
            public:
                SectorUpdateCallback( double &sunAz, double min, double max, unsigned int sunTextureUnit );
                //virtual bool cull(osg::NodeVisitor* nv, osg::Drawable *dbl, osg::State*) const ;
                virtual void update(osg::NodeVisitor* nv, osg::Drawable* dbl);

            protected:
                double &_sunAz;
                double _min, _max;
                unsigned int _sunTextureUnit;
                bool _withinDeg( double x, double min, double max ) const;
        };


        static double _range( double x, double r )
        {
            return x - (r * floor(x/r));
        }

        void _buildStateSet();

        double _findIncidenceLength(double);

        static unsigned int  _sunImageWidth;
	static unsigned int  _sunImageHeight;
        static unsigned int  _sunImageInternalTextureFormat;
        static unsigned int  _sunImagePixelFormat;
        static unsigned char _sunImageData[];

/* Sky luminance model adapted from:
 * A. J. Preetham, Peter, Shirley, Brian, Smits, "A Practical Analytic
 * Model for Daylight"
 * An implementation of Preetham is included here, but commented out.  This is
 * because the horizon and sunset colors from this model were very wrong.  Perhaps
 * this is a problem with the implementation, or perhaps the model.  The model in
 * use here is an empirical model by Terry Welsh, which is heavily influenced by
 * the Preetham model.
 */
        // Turbitity
        // The fraction of scattering due to haze rather than molecules
        // 2 = quite a clear day
        // 20 = hazy day
        // Reasonable values are 1 - 60
        float _T;

        // Angle between sun direction and zenith
        float _theta_sun, _theta_sun_0_1, _cos_theta_sun, _sin_theta_sun, _cos_theta_sun_squared;
 
        // Altitude at which total darkness occurs
        float _dark_alt;
        // How overall sky brightness varies with altitude
        float _day_exp;
        float _light_due_to_alt;  // in range {0,1}
        
        // sunset attenuation factor
        float _sunset_atten;
 
        // Luminance distribution function coefficients.
        // xyY is an accurate color representation designed by
        // the Commission Internationale de l'Eclairage (CIE).
        // Y is luminance, and x and y describe chromaticity (or color).
        float _Ax, _Bx, _Cx, _Dx, _Ex;
        float _Ay, _By, _Cy, _Dy, _Ey;
        float _AY, _BY, _CY, _DY, _EY;

        // Zenith color values in CIE xyY format
        float _xz, _yz, _Yz;

        // Terry's RGB coefficients
        // A = horizon intensity
        // B = horizon falloff
        // C = circumsolar intensity
        // D = circumsolar falloff
        // E = overall (linear) component
        float _Ar, _Br, _Cr, _Dr, _Er;
        float _Ag, _Bg, _Cg, _Dg, _Eg;
        float _Ab, _Bb, _Cb, _Db, _Eb;
        float _horiz_atten_r, _solar_atten_r;
        float _horiz_atten_g, _solar_atten_g;
        float _horiz_atten_b, _solar_atten_b;

        int _current_tex_row;

        virtual void _updateDistributionCoefficients();
        void _updateZenithxyY();
        inline float _xDistributionFunction(const float theta, const float cos_theta,
                                           const float gamma, const float cos_gamma_sq);
        inline float _yDistributionFunction(const float theta, const float cos_theta,
                                           const float gamma, const float cos_gamma_sq);
        inline float _YDistributionFunction(const float theta, const float cos_theta,
                                           const float gamma, const float cos_gamma_sq);
        inline float   _RedFunction(const float theta, const float theta_0_1,
                                          const float gamma, const float gamma_1_0);
        inline float _GreenFunction(const float theta, const float theta_0_1,
                                          const float gamma, const float gamma_1_0);
        inline float  _BlueFunction(const float theta, const float theta_0_1,
                                          const float gamma, const float gamma_1_0);
        void _computeSkyTexture();
};

/** RSC modifications for sensor simulation.
  */
inline
osg::Image*
SkyDome :: getSkyImage()
{
  return _skyTexture->getImage();
}

/** RSC modifications for sensor simulation.
  */
inline
void
SkyDome :: setSkyImage(osg::Image* image)
{
  _overrideSkyImage = true;
  
  _skyTexture->setImage(image);
}


}

#endif
