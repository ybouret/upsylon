//! \file

#ifndef Y_COLOR_HISTOGRAM_INCLUDED
#define Y_COLOR_HISTOGRAM_INCLUDED 1

#include "y/graphic/color/convert.hpp"

namespace upsylon
{

    namespace graphic
    {
        //! color histogram
        class histogram
        {
        public:
            // types and definitions
            static const size_t bins = 256;

            // C++
            virtual ~histogram() throw();
            explicit histogram() throw();
            histogram(const histogram &) throw();
            histogram & operator=( const histogram &) throw();

            // methods
            void           reset()                          throw();
            void           add(const size_t *)              throw();
            void           add(const histogram &)           throw();
            size_t       & operator[](const uint8_t )       throw();
            const size_t & operator[](const uint8_t ) const throw();

            template <typename T>
            histogram & operator<<(const T &data)
            {
                const uint8_t b = convert<uint8_t,T>:: from(data);
                ++bin[b];
                return *this;
            }


        private:
            size_t bin[bins];
        };
    }

}

#endif

