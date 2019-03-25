//! \file
#ifndef Y_IOS_IRSTREAM_INCLUDED
#define Y_IOS_IRSTREAM_INCLUDED 1


#include "y/ios/istream.hpp"
#include "y/fs/local-file.hpp"

namespace upsylon
{
    namespace ios
    {
        //! input raw stream
        class irstream : public istream, public local_file
        {
        public:
            //! open a file stream
            explicit irstream(const string &filename, const offset_t shift=0,const offset_t len=-1);

            //! open a file stream
            explicit irstream(const char   *filename, const offset_t shift=0,const offset_t len=-1);

            //! map from cstdin
            explicit irstream( const ios::cstdin_t & );

            //! destructor
            virtual ~irstream() throw();

            virtual bool query(char &C); //!< get one char
            virtual void store(char  C); //!< go back one char, no check

        private:
            Y_DISABLE_COPY_AND_ASSIGN(irstream);
            int             cache;
            offset_t        count;
            const offset_t  max_length;
        };
    }
}

#endif

