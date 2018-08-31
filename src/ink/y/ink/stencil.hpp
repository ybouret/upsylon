//! \file
#ifndef Y_INK_STENCIL_INCLUDED
#define Y_INK_STENCIL_INCLUDED 1

#include "y/container/matrix.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! Stencil builder interface
        template <typename T>
        class Stencil : public Object, public counted
        {
        public:
            const string name;

            //! destructor
            inline virtual ~Stencil() throw() {}

            virtual void build( matrix<T> &m ) const = 0;

        protected:
            //! constructor
            inline explicit Stencil(const char *id) : name(id) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Stencil);
        };

#define Y_INK_STENCIL_IMPL(NAME,CODE) \
template <typename T> class NAME : public Stencil<T> {\
public:\
inline explicit NAME() : Stencil<T>(#NAME) {}\
inline virtual ~NAME() throw() {}\
inline virtual void build( matrix<T> &m ) const { CODE; }\
private: Y_DISABLE_COPY_AND_ASSIGN(NAME);\
}

    }
}

#endif

