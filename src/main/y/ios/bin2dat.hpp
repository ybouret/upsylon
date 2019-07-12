//! \file
#ifndef Y_IOS_BIN2DAT_INCLUDED
#define Y_IOS_BIN2DAT_INCLUDED 1

#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace ios
    {

        //! binary to data wrapper
        class bin2dat
        {
        public:
            explicit bin2dat(const size_t w); //!< initialize with output width
            virtual ~bin2dat() throw();       //!< destructor
            void reset() throw();             //!< start new line
            void write(ios::ostream &fp, const char C, const bool isLast);     //!< output a char
            void write(ios::ostream &fp, const void *data, const size_t size); //!< output a block
            void write(ios::ostream &fp, const memory::ro_buffer &buf );       //!< output a block

        private:
            size_t       count;
            const size_t width;
            string      *human;
            
            Y_DISABLE_COPY_AND_ASSIGN(bin2dat);
        };
    }
}

#endif

