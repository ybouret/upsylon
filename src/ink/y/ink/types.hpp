//! \file
#ifndef Y_INK_TYPES_INCLUDED
#define Y_INK_TYPES_INCLUDED 1

#include "y/type/point2d.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! coordinate
        typedef point2d<unit_t> coord;
        //! complex type
        typedef complex<float>  cplx;
        
        //! red weight
#define Y_INK_R2GS 0.2126f
        //! green weight
#define Y_INK_G2GS 0.7152f
        //! blue weight
#define Y_INK_B2GS 0.0722f
        //! [0.0f:1.0f] to byte
#define Y_INK_F2B(x) (uint8_t(floorf((x)*255.0f+0.5f)))

        //! crux components and functions
        struct Crux
        {
            static const float u_float[256]; //!< unitfloat
            static const float r_float[256]; //!< red    unit float = Y_INK_R2GS*u_float
            static const float g_float[256]; //!< green  unit float = Y_INK_G2GS*u_float
            static const float b_float[256]; //!< blue   unit float = Y_INK_B2GS*u_float

            //! function float to byte
            static inline uint8_t FloatToByte( const float x) throw() { return Y_INK_F2B(x); }

            //! float to float
            static inline float FloatToFloat(const float x) throw() { return x; }
            

            //! convert to float grey scale
            static inline float   GreyScaleF( const uint8_t r, const uint8_t g, const uint8_t b) throw()
            {
                return r_float[r] + g_float[g] + b_float[b];
            }
            //! convert to byte greyscale
            static inline uint8_t  GreyScale1( const uint8_t r, const uint8_t g, const uint8_t b) throw()
            {
                return Y_INK_F2B(GreyScaleF(r,g,b));
            }
        };

        //! base class
        class Object : public object
        {
        public:
            virtual ~Object() throw();
            explicit Object() throw();
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Object);
        };
    }
}

#endif

