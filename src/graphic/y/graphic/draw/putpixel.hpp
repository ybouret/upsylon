//! \file

#ifndef Y_GRAPHIC_DRAW_PUTPIXEL
#define Y_GRAPHIC_DRAW_PUTPIXEL 1

#include "y/graphic/pixel.hpp"
#include "y/graphic/mask.hpp"

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
                    inline void operator()( T &target, const Point & ) const
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
                    inline void operator()( T &target, const Point & ) const
                    {
                        target = Pixel::Blend<T>( color, target, alpha );
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Blend);
                };

                //! storing coordinates
                class Store
                {
                public:
                    Mask &mask; //!< destination

                    Store( Mask &msk ) throw(); //!< setup
                    ~Store() throw();           //!< cleanup
                    
                    //! insert coordinate into mask
                    template <typename T>
                    inline void operator()( T &, const Point &p ) const
                    {
                        mask.insert(p);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Store);
                };
            };

        }

    }
}

#endif
