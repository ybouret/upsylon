//! \file

#ifndef Y_GRAPHIC_PIXMAP_INCLUDED
#define Y_GRAPHIC_PIXMAP_INCLUDED 1

#include "y/graphic/surface.hpp"
#include "y/type/args.hpp"

namespace upsylon {

    namespace Graphic {

        //! ABI for AnonymousRow
        template <typename T>
        class Row
        {
        public:
            Y_DECL_ARGS(T,type);

        private:
            type              *addr;

        public:
            const Bitmap      &bitmap;

            inline const_type & operator[]( const unit_t i ) const throw()
            {
                assert(addr);
                assert(i>=0);
                assert(i<bitmap.w);
                return addr[i];
            }

        private:
            Row(); ~Row() throw();
            Y_DISABLE_COPY_AND_ASSIGN(Row);
        };

        template <typename T>
        class Pixmap : public Surface
        {
        public:
            Y_DECL_ARGS(T,type);

            inline explicit Pixmap(const size_t W, const size_t H) :
            Surface( Bitmap::Create(W,H,sizeof(T)) )
            {
            }

            inline virtual ~Pixmap() throw()
            {
            }

            inline const Row<T> & operator[](const unit_t j) const throw()
            {
                const  Bitmap           &self = **this;
                return *(const Row<T> *)(self.getStandard(j));
            }

            inline const Row<T> & operator()(const unit_t j) const throw()
            {
                const  Bitmap &self = **this;
                return *(const Row<T> *)(self.getZeroFlux(j));
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pixmap);
        };

    }
    
}

#endif

