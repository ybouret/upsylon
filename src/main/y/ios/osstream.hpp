//! \file
#ifndef Y_OSSTREAM_INCLUDED
#define Y_OSSTREAM_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace ios
    {
        //! output string stream
        class osstream : public ostream
        {
        public:
            
            //! destructor
            virtual ~osstream() throw();
            
            //! temporary output stream on a more persistent string
            explicit osstream( string &s ) throw();

            //! write
            void write(char C);

            //! do nothing...
            virtual void flush();
            
        private:
            string &host;
            Y_DISABLE_COPY_AND_ASSIGN(osstream);
        };

        //! with own string
        class embedded_osstream : public string, public osstream
        {
        public:
            explicit embedded_osstream();
            virtual ~embedded_osstream() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(embedded_osstream);
        };

    }
}

#endif
