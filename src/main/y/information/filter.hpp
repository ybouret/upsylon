//! \file
#ifndef Y_INFORMATION_FILTER_INCLUDED
#define Y_INFORMATION_FILTER_INCLUDED 1

#include "y/sequence/list.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace information {


        //! filter base class
        /**
         the ostream part must implement the data transformation and storage
         that can be retrieved by the ostream part
         */
        class filter : public ios::istream, public ios::ostream
        {
        public:
            virtual     ~filter() throw();     //!< cleanup
            virtual void reset()  throw() = 0; //!< reset state

            //! process from current state
            void   process(ios::ostream &target, ios::istream &source);
            string to_string(ios::istream &source);
            string to_string(const char   *text);
            string to_string(const memory::ro_buffer &buff);

        protected:
            explicit filter() throw(); //!< setup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(filter);
        };

    

    }

}

#endif

