//! \file

#ifndef Y_CORE_TEMPORARY_ACQUIRE_INCLUDED
#define Y_CORE_TEMPORARY_ACQUIRE_INCLUDED 1

#include "y/type/releasable.hpp"

namespace upsylon {

    namespace core
    {

        class temporary_acquire_
        {
        public:
            virtual ~temporary_acquire_() throw();
            void     record(releasable &obj) throw();

        protected:
            explicit temporary_acquire_(releasable **arr, const size_t num) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(temporary_acquire_);
            releasable   **addr;
            const size_t   capa;
        public:
            const size_t   size; //!< current number or recorded objects

        };

        template <size_t N>
        class temporary_acquire : public temporary_acquire_
        {
        public:
            static const size_t capacity = N;

            inline virtual ~temporary_acquire() throw() {}

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
