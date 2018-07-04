//! \file
#ifndef Y_CONTAINER_INCLUDED
#define Y_CONTAINER_INCLUDED 1

#include "y/dynamic.hpp"
#include "y/object.hpp"

namespace upsylon
{

    class container : public virtual dynamic, public virtual object
    {
    public:
        virtual ~container() throw();

        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________
        //! free active items
        virtual void free()    throw() = 0;
        //! free active items and release all attached memory
        virtual void release() throw() = 0;
        //! reserve memory for n items
        virtual void reserve(const size_t n) = 0;


        //______________________________________________________________________
        //
        // non-virtual interface
        //______________________________________________________________________

        //! ensure the total capacity is greater or equal to max_items
        inline void ensure(const size_t max_items)
        {
            if(max_items>capacity())
            {
                reserve(max_items-capacity());
            }
        }

        //! automatic memory reservation
        inline void next_capacity()
        {
            reserve( estimate_next_capacity(capacity())-capacity() );
        }
        
        //! estimate next capacity
        static size_t estimate_next_capacity(const size_t n);

    protected:
        explicit container() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(container);
    };

}

#endif

