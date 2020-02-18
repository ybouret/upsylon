//! \file

#ifndef Y_CORE_CHAINABLE_INCLUDED
#define Y_CORE_CHAINABLE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace core {

        //! base class for chainable object
        class chainable
        {
        public:
            virtual ~chainable() throw(); //!< cleanup


        protected:
            explicit chainable() throw(); //!< setup
            void     exceeds_capacity() const; //!< throw exception

        private:
            Y_DISABLE_COPY_AND_ASSIGN(chainable);
        };
    }

    //! a chainable object
    template <typename T>
    class chainable : public core::chainable
    {
    public:
        inline virtual ~chainable() throw() {} //!< cleanup

        //! memory management function
        virtual size_t put(const T *addr, const size_t size) = 0;

        //! piecewise block insertion
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
        inline explicit chainable() throw() {} //!< setup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(chainable);
    };


}

#endif


