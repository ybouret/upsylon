//! \file
#ifndef Y_INFORMATION_FILTER_INCLUDED
#define Y_INFORMATION_FILTER_INCLUDED 1

#include "y/sequence/list.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace information {


        class filter : public ios::istream, public ios::ostream
        {
        public:
            virtual ~filter() throw();

        protected:
            explicit filter() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(filter);
        };

        //! implement the istream part
        class q_filter : public filter, public list<char>
        {
        public:
            explicit q_filter() throw();
            virtual ~q_filter() throw();
            
            virtual bool query( char &C );
            virtual void store( char  C );
            virtual bool is_active() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(q_filter);
        };

        class echo_filter : public q_filter
        {
        public:
            explicit echo_filter() throw();
            virtual ~echo_filter() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(echo_filter);
        };

    }

}

#endif

