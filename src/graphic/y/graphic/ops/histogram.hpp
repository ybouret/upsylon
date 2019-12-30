//! \file

#ifndef Y_GRAPHIC_HISTOGRAM_INCLUDED
#define Y_GRAPHIC_HISTOGRAM_INCLUDED 1

#include "y/graphic/types.hpp"

namespace upsylon {

    namespace Graphic {

        class Histogram : public Object
        {
        public:
            static const size_t BINS = 256;

            explicit Histogram() throw();
            virtual ~Histogram() throw();

            void zero() throw();

            size_t &       operator[]( const uint8_t u ) throw();
            const size_t & operator[]( const uint8_t u ) const throw();

            //! bins[BINS]
            void set_(const size_t *bins) throw();

            //! bins[BINS]
            void add_(const size_t *bins) throw();

            void set( const Histogram &H ) throw();
            void add( const Histogram &H ) throw();

            uint8_t Otsu1D() const throw();

            size_t count() const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            size_t bin[BINS];
        };

    }

}

#endif

