//! \file

#ifndef Y_CORE_CHAINABLE_INCLUDED
#define Y_CORE_CHAINABLE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace core {

        class chainable
        {
        public:
            virtual ~chainable() throw();


        protected:
            explicit chainable() throw();
            void     exceeds_capacity() const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(chainable);
        };
    }

    template <typename T>
    class chainable : public core::chainable
    {
    public:
        inline virtual ~chainable() throw() {}

        virtual size_t put(const T *addr, const size_t size) = 0;
        
        inline void    put_all(const T *addr, const size_t size)
        {
            assert(!(0==addr&&size>0));
            size_t   todo = size;
            const T *curr = addr;
            while( todo > 0 )
            {
                const size_t count = put(curr,todo); assert(count<=todo);
                if(count<=0) exceeds_capacity();
                curr += count;
                todo -= count;
            }
        }


    protected:
        inline explicit chainable() throw() {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(chainable);
    };


}

#endif


