//! \file

#ifndef Y_IOS_SCRIBE_TUPLE_INCLUDED
#define Y_IOS_SCRIBE_TUPLE_INCLUDED 1

#include "y/ios/scribe/native.hpp"

namespace upsylon
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! scribe with multiple items
        //
        //______________________________________________________________________
        class scribeND : public scribe
        {
        public:
            virtual       ~scribeND() throw();                     //!< cleanup
            virtual string write(const void *addr) const throw();  //!< tuple writer

            const scribe1D &base; //!< for one item

        protected:
            //! setup
            explicit scribeND(const std::type_info &tid,
                              const scribe1D       &one,
                              const size_t          num);

            //! check, throw on error
            void check_base_is(const std::type_info &) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribeND);
        };

        //______________________________________________________________________
        //
        //
        //! tuple base
        //
        //______________________________________________________________________
        template <
        template <class> class TUPLE,
        typename T>
        class tuple_scribe : public scribeND
        {
        public:
            //! cleanup
            inline virtual ~tuple_scribe() throw() {}

            //! setup
            inline explicit tuple_scribe(const scribe1D &one) :
            scribeND( typeid( TUPLE<T> ), one, sizeof(TUPLE<T>)/sizeof(T) )
            {
                check_base_is( typeid(T) );
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tuple_scribe);
        };

    }

}


#endif
