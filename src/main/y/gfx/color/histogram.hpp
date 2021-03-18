//! \file

#ifndef Y_COLOR_HISTOGRAM_INCLUDED
#define Y_COLOR_HISTOGRAM_INCLUDED 1

#include "y/gfx/color/convert.hpp"
#include "y/strfwd.hpp"

namespace upsylon
{

    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! color histogram
        //
        //______________________________________________________________________
        class histogram
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const size_t bins = 256; //!< using bytes as index

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~histogram() throw();                      //!< cleanup
            explicit histogram() throw();                      //!< setup
            histogram(const histogram &) throw();              //!< copy
            histogram & operator=( const histogram &) throw(); //!< assign

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void           reset()                          throw(); //!< to zero
            void           add(const size_t *)              throw(); //!< [0..255] array udate
            void           add(const histogram &)           throw(); //!< add content
            size_t       & operator[](const uint8_t )       throw(); //!< access
            const size_t & operator[](const uint8_t ) const throw(); //!< access, const

            //! register a pixel using default type conversion
            template <typename T>
            histogram & operator<<(const T &data)
            {
                const uint8_t b = convert<uint8_t,T>:: from(data);
                ++bin[b];
                return *this;
            }

            //! save to file
            void save(const string &filename) const;
            
            //! test equality
            friend bool operator==( const histogram &lhs, const histogram &rhs) throw();

            //! test difference
            friend bool operator!=( const histogram &lhs, const histogram &rhs) throw();

            //! helper, mostly to debug
            size_t private_bytes() const throw();

            //! Otsu's threshold
            uint8_t threshold() const throw();

        private:
            size_t bin[bins];
        };
    }

}

#endif

