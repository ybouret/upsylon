
//! \file
#ifndef Y_INFORMATION_Q_FILTER_INCLUDED
#define Y_INFORMATION_Q_FILTER_INCLUDED 1

#include "y/information/filter.hpp"
#include "y/sequence/list.hpp"

namespace upsylon {

    namespace information {

        //! implement the istream part
        class filterQ : public filter, public list<char>
        {
        public:
            explicit filterQ() throw();      //!< create empty
            explicit filterQ(const size_t);  //!< create with memory
            virtual ~filterQ() throw();      //!< cleanup

            virtual bool query( char &C );    //!< from front()
            virtual void store( char  C );    //!< push front
            virtual bool is_active() throw(); //!< not empty

        private:
            Y_DISABLE_COPY_AND_ASSIGN(filterQ);
        };
        
    }

}

#endif
