//! \file
#ifndef Y_INFORMATION_QBYTES_INCLUDED
#define Y_INFORMATION_QBYTES_INCLUDED 1

#include "y/sequence/list.hpp"
#include "y/ios/istream.hpp"

namespace upsylon {

    namespace information {

        class qbytes : public list<char>, public ios::istream
        {
        public:
            explicit qbytes() throw();
            virtual ~qbytes() throw();
            
            virtual bool query( char &C );
            virtual void store( char  C );
            virtual bool is_active() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(qbytes);
        };

    }

}

#endif

