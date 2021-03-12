
//! \file

#ifndef Y_GFX_PIXROW_INCLUDED
#define Y_GFX_PIXROW_INCLUDED 1

#include "y/gfx/types.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //! row for pixel
        //______________________________________________________________________
        template <typename T>
        class pixrow
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type); //!< alias

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! raw access
            inline type & operator()(const unit_t i) throw()
            {
                assert(p);
                assert(i>=0); assert(i<w); return p[i];
            }

            //! raw access, const
            inline const_type & operator()(const unit_t i) const throw()
            {
                assert(p);
                assert(i>=0); assert(i<w); return p[i];
            }

            //! zero-flux access
            inline type & operator[](const unit_t i) throw()
            {
                assert(p);
                std::cerr << "@x=" << i << "->" << z(i) << std::endl;
                return p[z(i)];
            }

            //! zero-flux access, const
            inline const_type & operator[](const unit_t i) const throw()
            {
                assert(p);
                return p[z(i)];
            }


        private:
            pixrow() throw(); ~pixrow() throw();
            Y_DISABLE_COPY_AND_ASSIGN(pixrow);
            mutable_type *p;
        public:
            const unit_t  w; //!< width
            const zflux  &z; //!< host zero-flux for width
        };
    }

}

#endif
