//! \file

#ifndef Y_INK_STENCIL_INCLUDED
#define Y_INK_STENCIL_INCLUDED 1

#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! wrapper around matrix
        template <typename T>
        class Stencil
        {
        public:
            const matrix<T> weights;

            inline virtual ~Stencil() throw() {}


            inline const array<T> &operator[](size_t y) const throw() { return weights[y]; }

        protected:
            inline Stencil(const size_t w, const size_t h) :
            weights(h,w)
            {
            }
            
        private:
            Y_DISABLE_ASSIGN(Stencil);
        };
    }
}

#endif

