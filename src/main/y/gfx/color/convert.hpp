//! \file

#ifndef Y_COLOR_CONVERT_INCLUDED
#define Y_COLOR_CONVERT_INCLUDED 1

#include "y/gfx/color/rgb.hpp"
#include "y/gfx/color/yuv.hpp"
#include <cmath>

namespace upsylon
{

    namespace graphic
    {
        
        namespace crux
        {
            //__________________________________________________________________
            //
            //! basic conversion helpers
            //__________________________________________________________________
            struct convert
            {
                static const  float   unit_float[256]; //!< 0/255,...255/255

                //! [0:1] -> [0:255]
                static inline uint8_t to_byte(const float f) throw()
                {
                    return uint8_t( floorf(255.0f*f+0.5f) );
                }

                //! greyscale compression table
                static  const uint8_t greyscale[255*3+1];

                //! fg:alpha, bg:beta, beta=255-alpha
                static uint8_t mix(const uint8_t fg,
                                   const uint8_t bg,
                                   const uint8_t alpha,
                                   const uint8_t beta) throw();

            };
        }

        //______________________________________________________________________
        //
        //
        //! built-in conversion
        //
        //______________________________________________________________________
        template <typename U, typename T>
        struct convert
        {
            //! T->U
            static U from(const T &x) throw();
        };

        //______________________________________________________________________
        //
        //
        //! built-in auto-conversion
        //
        //______________________________________________________________________
        template <typename T>
        struct convert<T,T>
        {
            //! T->T
            static const T & from(const T &x) throw() { return x; }
        };
        
        template <typename T>
        inline const T & identity(const T &x) throw() { return x; }

        //______________________________________________________________________
        //
        //
        //! converter base class
        //
        //______________________________________________________________________
        class converter
        {
        public:
            virtual       ~converter() throw();       //!< cleanup
            virtual unit_t depth() const throw() = 0; //!< target/source depth
            
            
        protected:
            explicit       converter() throw();       //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(converter);
        };
    }


}

#endif
