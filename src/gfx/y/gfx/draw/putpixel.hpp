//! \file

#ifndef Y_GRAPHIC_DRAW_PUTPIXEL
#define Y_GRAPHIC_DRAW_PUTPIXEL 1

#include "y/gfx/pixel/blend.hpp"
#include "y/gfx/mask.hpp"
#include "y/type/authority.hpp"

namespace  upsylon {

    namespace graphic {

        namespace draw {

            //! put pixel wrappers
            struct putpixel
            {
                //! will copy content
                template <typename T>
                class copy
                {
                public:
                    const T color; //!< the new foreground color

                    //! setup
                    inline  copy( const T &value ) : color(value) {}

                    //!cleanup
                    inline ~copy() throw() {} //!< cleanup

                    //! call: set target to color
                    inline void operator()( T &target, const coord & ) const
                    {
                        target = color;
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(copy);
                };

                //! will blend color with background
                template <typename T>
                class blend
                {
                public:
                    const T       color; //!< the foreground color
                    const uint8_t alpha; //!< the alpha value

                    //! setup
                    inline  blend( const T &value, const uint8_t a ) : color(value), alpha(a) {}

                    //! cleanup
                    inline ~blend() throw() {}

                    //! call: blend color with background target
                    inline void operator()( T &target, const coord & ) const
                    {
                        target = graphic::blend<uint8_t,T>::mix(alpha,color,target);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(blend);
                };

                //! storing coordinates
                class store : public authority<mask>
                {
                public:

                    explicit store(mask &) throw(); //!< setup
                    virtual ~store()       throw(); //!< cleanup
                    
                    //! insert coordinate into mask
                    template <typename T>
                    inline void operator()( T &, const coord &p )  
                    {
                      (**this).insert(p);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(store);
                };
            };

        }

    }
}

#endif
