//! \file
#ifndef Y_IOS_IRSTREAM_INCLUDED
#define Y_IOS_IRSTREAM_INCLUDED 1


#include "y/ios/istream.hpp"
#include "y/fs/local-file.hpp"

namespace upsylon
{
    namespace ios
    {

        class irstream : public istream, public local_file
        {
        public:
            explicit irstream(const string &filename, const offset_t shift=0);
            explicit irstream(const char   *filename, const offset_t shift=0);
            virtual ~irstream() throw();

            virtual bool query(char &C);
            virtual void store(char  C);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(irstream);
        };
    }
}

#endif

