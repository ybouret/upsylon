//! \file

#ifndef Y_MOMENTARY_ACQUIRE_INCLUDED
#define Y_MOMENTARY_ACQUIRE_INCLUDED 1

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
        class momentary_acquire_
        {
        public:
            virtual ~momentary_acquire_() throw();                 //!< release_all() and cleanup
            void     record(releasable &) throw();                 //!< record a new object
            momentary_acquire_ & operator<<(releasable &) throw(); //!< helper
            void release_all()                            throw(); //!< release all in reverse recorded order

        protected:
            //! setup from user's defined memory
            explicit momentary_acquire_(releasable **arr, const size_t num) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(momentary_acquire_);
            releasable   **addr;
            const size_t   capa;
        public:
            const size_t   size; //!< current number or recorded objects
        };


    }

    //__________________________________________________________________________
    //
    //
    //! fixed count local temporary acquired
    //
    //__________________________________________________________________________
    template <size_t N>
    class momentary_acquire : public core::momentary_acquire_
    {
    public:
        static const size_t capacity = N; //!< alias

        //! cleanup
        inline virtual ~momentary_acquire() throw() {}

        //! setup
        inline explicit momentary_acquire() throw() :
        core::momentary_acquire_(prv,capacity)
        {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(momentary_acquire);
        releasable *prv[N];
    };



}

#endif
