
//! \file
#ifndef Y_TYPE_SELF_DESTRUCT_INCLUDED
#define Y_TYPE_SELF_DESTRUCT_INCLUDED 1

#include "y/type/traits.hpp"

namespace upsylon
{

    //! self-destruct functions
    class self_destruct
    {
    public:
        //! in place destructor
        template <typename T>
        static inline void on( T &data ) throw()
        {
            typedef typename type_traits<T>::mutable_type mutable_T;
            mutable_T *item = (mutable_T *)&data;
            item->~mutable_T();
        }
        
        //! in place destructor
        template <typename T>
        static inline T *at(T *data) throw()
        {
            assert(data);
            on(*data);
            return data;
        }
        
    private:
        self_destruct(); ~self_destruct();
        Y_DISABLE_COPY_AND_ASSIGN(self_destruct);
    };
    
   
    
  
    
}

#endif

