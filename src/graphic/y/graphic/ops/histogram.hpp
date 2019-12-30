//! \file

#ifndef Y_GRAPHIC_HISTOGRAM_INCLUDED
#define Y_GRAPHIC_HISTOGRAM_INCLUDED 1

#include "y/graphic/types.hpp"

namespace upsylon {

    namespace Graphic {

        //! histogram of bytes
        class Histogram : public Object
        {
        public:
            static const size_t BINS = 256; //!< number of bins

            explicit Histogram() throw(); //!< setup
            virtual ~Histogram() throw(); //!< cleanup

            void zero() throw(); //!< set to zero

            size_t &       operator[]( const uint8_t u ) throw();       //!< access
            const size_t & operator[]( const uint8_t u ) const throw(); //!< const access

            //! bins[BINS], unsafe
            void set_(const size_t *bins) throw();

            //! bins[BINS], unsafe
            void add_(const size_t *bins) throw();

            //! set to another histogram
            void set( const Histogram &H ) throw();

            //! add another histogram
            void add( const Histogram &H ) throw();

            //! Otsu 1D thresholding
            uint8_t Otsu1D() const throw();

            //! total count
            size_t count() const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            size_t bin[BINS];
        };

    }

}

#endif

