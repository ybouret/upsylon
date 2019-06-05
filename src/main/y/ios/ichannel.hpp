
//! \file
#ifndef Y_IOS_ICHANNEL_INCLUDED
#define Y_IOS_ICHANNEL_INCLUDED 1


#include "y/ios/istream.hpp"
#include "y/fs/local/file.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace ios
    {

        //! low level re-usable memory
        class bulk : public counted_object
        {
        public:
            typedef arc_ptr<bulk> pointer; //!< alias

            size_t size; //!< available bytes
            char  *addr; //!< first available bytes
            explicit bulk(const size_t n); //!< make size>=n
            virtual ~bulk() throw();       //!< destructor
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(bulk);
        };


        //! buffered local_file
        class ichannel : public istream
        {
        public:
            //! destructor
            virtual ~ichannel() throw();

            //! open a file stream
            explicit ichannel(const string        &filename,
                              const bulk::pointer &buff,
                              const offset_t       shift=0);

            //! open a file stream
            explicit ichannel(const char          *filename,
                              const bulk::pointer &buff,
                              const offset_t       shift=0);

            //! map from cstdin
            explicit ichannel( const cstdin_t     &,
                              const bulk::pointer &buff);

            virtual bool query(char &C); //!< get one char
            virtual void store(char  C); //!< unread

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ichannel);
            char         *curr;
            char         *last;
            bulk::pointer io;
            local_file    fp;
            bool          reload();
        };
    }

}

#endif

