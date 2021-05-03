

//! \file

#ifndef Y_IOS_SCRIBE_TUPLE_INCLUDED
#define Y_IOS_SCRIBE_TUPLE_INCLUDED 1

#include "y/ios/scribe/native.hpp"

namespace upsylon
{
    namespace ios
    {
        class scribeND : public scribe
        {
        public:
            virtual ~scribeND() throw();

            const scribe1D &base;

            virtual string write(const void *addr) const throw();


        protected:
            explicit scribeND(const std::type_info &tid,
                              const scribe1D       &one,
                              const size_t          num);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribeND);
        };

        template <
        template <class> class TUPLE,
        typename T>
        class tuple_scribe : public scribeND
        {
        public:
            inline virtual ~tuple_scribe() throw() {}

            inline explicit tuple_scribe(const scribe1D &one) :
            scribeND( typeid( TUPLE<T> ), one, sizeof(TUPLE<T>)/sizeof(T) )
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tuple_scribe);
        };

    }

}


#endif
