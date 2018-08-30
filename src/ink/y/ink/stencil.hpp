//! \file
#ifndef Y_INK_STENCIL_INCLUDED
#define Y_INK_STENCIL_INCLUDED 1

#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! Stencil builder interface
        template <typename T>
        class Stencil : public counted_object
        {
        public:
            //! destructor
            inline virtual ~Stencil() throw() {}

            virtual void build( matrix<T> &m ) const = 0;

        protected:
            //! constructor
            inline explicit Stencil() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Stencil);
        };

#define Y_INK_STENCIL_IMPL(NAME,CODE) \
template <typename T> class NAME : public Stencil<T> {\
public:\
inline explicit NAME() throw() {}\
inline virtual ~NAME() throw() {}\
inline virtual void build( matrix<T> &m ) const { CODE; }\
private: Y_DISABLE_COPY_AND_ASSIGN(NAME);\
}

    }
}

#endif

