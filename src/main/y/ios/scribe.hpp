//! \file

#ifndef Y_IOS_SCRIBE_INCLUDED
#define Y_IOS_SCRIBE_INCLUDED 1

#include "y/ios/ostream.hpp"
#include <typeinfo>

namespace upsylon {

    namespace ios {

        class scribe : public counted_object
        {
        public:
            virtual ~scribe() throw();

            const std::type_info &info;
            const size_t          count;

            template <typename T>
            ios::ostream & put( ios::ostream &os, const T &args ) const
            {
                assert( typeid(T) == info );
                write(os,&args);
                return os;
            }


        protected:
            explicit scribe(const std::type_info &, const size_t) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribe);
            virtual void write( ios::ostream &, const void * ) const = 0;
        };

    }
}

#endif

