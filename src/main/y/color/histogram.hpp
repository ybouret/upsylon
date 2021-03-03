//! \file

#ifndef Y_COLOR_HISTOGRAM_INCLUDED
#define Y_COLOR_HISTOGRAM_INCLUDED 1

#include "y/color/convert.hpp"

namespace upsylon
{

    //! color histogram
    class color_histogram
    {
    public:
        // types and definitions
        static const size_t bins = 256;

        // C++
        virtual ~color_histogram() throw();
        explicit color_histogram() throw();
        color_histogram(const color_histogram &) throw();
        color_histogram & operator=( const color_histogram &) throw();

        // methods
        void           reset()                          throw();
        void           add(const size_t *)              throw();
        void           add(const color_histogram &)     throw();
        size_t       & operator[](const uint8_t )       throw();
        const size_t & operator[](const uint8_t ) const throw();

        template <typename T>
        color_histogram & operator<<(const T &data)
        {
            const uint8_t b = convert_color<uint8_t,T>:: from(data);
            ++bin[b];
            return *this;
        }

        
    private:
        size_t bin[bins];
    };

}

#endif

