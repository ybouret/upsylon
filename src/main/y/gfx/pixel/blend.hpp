//! \file


#ifndef Y_GFX_BLEND_INCLUDED
#define Y_GFX_BLEND_INCLUDED 1

#include "y/gfx/color/rgb.hpp"
#include "y/gfx/color/convert.hpp"

#include <cmath>

namespace upsylon
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //! templates to implement 'mix' function
        //______________________________________________________________________
        template <typename ALPHA, typename T>
        struct blend;

        //______________________________________________________________________
        //
        //! float with float => straighforward
        //______________________________________________________________________
        template <>
        struct blend<float,float>
        {
            //! floating point formula
            static inline float mix(const float alpha, const float fg, const float bg) throw()
            {
                return bg + alpha*(fg-bg);
            }
        };

        //______________________________________________________________________
        //
        //! blending with bytes
        //______________________________________________________________________
        template <>
        struct blend<uint8_t,uint8_t>
        {
            //! precomputed table
            static const int   * const shift; //!< int[-255..255]

            //! alpha * fg + (255-alpha) *bg
            static inline
            uint8_t mix(const uint8_t alpha,
                        const uint8_t fg,
                        const uint8_t bg) throw()
            {
                switch(alpha)
                {
                    case 0x00: return bg;
                    case 0xff: return fg;
                    case 0x80: return uint8_t((unsigned(fg)+unsigned(bg))>>1);
                    default:
                        break;
                }
                const int FG = int ( unsigned(fg) << 8 );
                const int BG = int(alpha) * shift[ int(fg) - int(bg) ];
                return uint8_t(unsigned(FG+BG)>>8);
            }
        };

        //______________________________________________________________________
        //
        //! blending bytes with float
        //______________________________________________________________________
        template <>
        struct blend<float,uint8_t>
        {
            //! precomputed table
            static const float   * const shift; //!< int[-255..255]

            //! alpha * fg + (1-alpha) * bg
            static inline
            uint8_t mix(const float   alpha,
                        const uint8_t fg,
                        const uint8_t bg) throw()
            {
                const float resf = shift[bg] + alpha * shift[ int(fg) - int(bg) ];
                return uint8_t( floorf(resf+0.5f) );
            }

        };

        //______________________________________________________________________
        //
        //! blending floats with  byte
        //______________________________________________________________________
        template <>
        struct blend<uint8_t,float>
        {
            
            //! alpha * fg + (1-alpha) * bg
            static inline
            float mix(const uint8_t alpha,
                      const float   fg,
                      const float   bg) throw()
            {
                const float wf = crux::convert::unit_float[alpha];
                const float wb = crux::convert::unit_float[0xff-alpha];
                return fg*wf + bg*wb;
            }

        };



        //______________________________________________________________________
        //
        //! blending rgb with byte
        //______________________________________________________________________
        template <>
        struct blend<uint8_t,rgb>
        {
            typedef blend<uint8_t,uint8_t> _; //!< bytewise ops

            //! mix (r,g,b)
            static inline
            rgb mix(const uint8_t alpha, const rgb fg, const rgb bg) throw()
            {
                return rgb(_::mix(alpha,fg.r,bg.r),
                           _::mix(alpha,fg.g,bg.g),
                           _::mix(alpha,fg.b,bg.b));
            }

        };

        //______________________________________________________________________
        //
        //! blending rgba with byte
        //______________________________________________________________________
        template <>
        struct blend<uint8_t,rgba>
        {
            typedef blend<uint8_t,uint8_t> _; //!< bytewise ops

            //! mix (r,g,b) only
            static inline
            rgba mix(const uint8_t alpha, const rgba fg, const rgba bg) throw()
            {
                return rgba(_::mix(alpha,fg.r,bg.r),
                            _::mix(alpha,fg.g,bg.g),
                            _::mix(alpha,fg.b,bg.b));
            }

        };

        //______________________________________________________________________
        //
        //! blending rgb with float
        //______________________________________________________________________
        template <>
        struct blend<float,rgb>
        {
            typedef blend<float,uint8_t> _; //!< bytewise ops

            //! mix (r,g,b)
            static inline
            rgb mix(const float alpha, const rgb fg, const rgb bg) throw()
            {
                return rgb(_::mix(alpha,fg.r,bg.r),
                           _::mix(alpha,fg.g,bg.g),
                           _::mix(alpha,fg.b,bg.b));
            }

        };

        //______________________________________________________________________
        //
        //! blending rgba with float
        //______________________________________________________________________
        template <>
        struct blend<float,rgba>
        {
            typedef blend<float,uint8_t> _; //!< bytewise ops

            //! mix (r,g,b) only
            static inline
            rgba mix(const float alpha, const rgba fg, const rgba bg) throw()
            {
                return rgba(_::mix(alpha,fg.r,bg.r),
                            _::mix(alpha,fg.g,bg.g),
                            _::mix(alpha,fg.b,bg.b));
            }

        };

        

    }

}

#endif

