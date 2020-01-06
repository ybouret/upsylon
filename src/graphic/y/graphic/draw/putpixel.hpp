//! \file

#ifndef Y_GRAPHIC_DRAW_PUTPIXEL
#define Y_GRAPHIC_DRAW_PUTPIXEL 1

#include "y/graphic/pixel.hpp"

namespace  upsylon {

    namespace Graphic {

        namespace Draw {

            struct PutPixel
            {
                template <typename T>
                class Copy
                {
                public:
                    const T color;
                    inline  Copy( const T &value ) : color(value) {}
                    inline ~Copy() throw() {}

                    inline void operator()( T &target ) const
                    {
                        target = color;
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Copy);
                };

                template <typename T>
                class Blend
                {
                public:
                    const T       color;
                    const uint8_t alpha;

                    inline  Blend( const T &value, const uint8_t a ) : color(value), alpha(a) {}
                    inline ~Blend() throw() {}

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
