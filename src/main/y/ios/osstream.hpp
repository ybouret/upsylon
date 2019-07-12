//! \file
#ifndef Y_OSSTREAM_INCLUDED
#define Y_OSSTREAM_INCLUDED 1

#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace ios
    {
        //! output string stream
        class osstream : public ostream
        {
        public:
            //! destructor
            inline virtual ~osstream() throw() {}
            
            //! temporary output stream on a more persistent string
            inline explicit osstream( string &s ) throw() : host(s) {}

            //! write
            void write(char C);

            //! do nothing...
            inline virtual void flush(){}

        private:
            string &host;
            Y_DISABLE_COPY_AND_ASSIGN(osstream);
        };
    }
}

#endif
