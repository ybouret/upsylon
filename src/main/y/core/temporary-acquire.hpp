//! \file

#ifndef Y_CORE_TEMPORARY_ACQUIRE_INCLUDED
#define Y_CORE_TEMPORARY_ACQUIRE_INCLUDED 1

#include "y/type/releasable.hpp"

namespace upsylon {

    namespace core
    {

        //______________________________________________________________________
        //
        //
        //! base class to localy release objects
        //
        //______________________________________________________________________
        class temporary_acquire_
        {
        public:
            virtual ~temporary_acquire_() throw();      //!< release() for all recorded objects
            void     record(releasable &obj) throw();   //!< record a new object

        protected:
            //! setup from user's defined memory
            explicit temporary_acquire_(releasable **arr, const size_t num) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(temporary_acquire_);
            releasable   **addr;
            const size_t   capa;
        public:
            const size_t   size; //!< current number or recorded objects

        };


        //______________________________________________________________________
        //
        //
        //! fixed count local temporary acquired
        //
        //______________________________________________________________________
        template <size_t N>
        class temporary_acquire : public temporary_acquire_
        {
        public:
            static const size_t capacity = N; //!< alias

            //! cleanup
            inline virtual ~temporary_acquire() throw() {}

            //! setup
            inline explicit temporary_acquire() throw() :
            temporary_acquire_(prv,capacity)
            {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(temporary_acquire);
            releasable *prv[N];
        };

    }

}

#endif
