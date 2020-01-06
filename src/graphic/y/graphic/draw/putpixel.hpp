//! \file

#ifndef Y_GRAPHIC_DRAW_PUTPIXEL
#define Y_GRAPHIC_DRAW_PUTPIXEL 1

#include "y/graphic/pixel.hpp"

namespace  upsylon {

    namespace Graphic {

        namespace Draw {

            //! put pixel wrappers
            struct PutPixel
            {
                //! will copy content
                template <typename T>
                class Copy
                {
                public:
                    const T color; //!< the new foreground color

                    //! setup
                    inline  Copy( const T &value ) : color(value) {}

                    //!cleanup
                    inline ~Copy() throw() {} //!< cleanup

                    //! call: set target to color
                    inline void operator()( T &target ) const
                    {
                        target = color;
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Copy);
                };

                //! will blend color with background
                template <typename T>
                class Blend
                {
                public:
                    const T       color; //!< the foreground color
                    const uint8_t alpha; //!< the alpha value

                    //! setup
                    inline  Blend( const T &value, const uint8_t a ) : color(value), alpha(a) {}

                    //! cleanup
                    inline ~Blend() throw() {}

                    //! call: blend color with background target
                    inline void operator()( T &target ) const
                    {
                        target = Pixel::Blend<T>( color, target, alpha );
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Blend);
                };
            };

        }

    }
}

#endif
