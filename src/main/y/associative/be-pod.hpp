
//! \file

#ifndef Y_BE_POD_INCLUDED
#define Y_BE_POD_INCLUDED 1

#include "y/memory/buffer.hpp"
#include "y/type/args.hpp"
#include "y/code/round.hpp"

#include <cstring>

namespace upsylon {

    template <typename T, const size_t N>
    class BEpod : public memory::ro_buffer
    {
    public:
        static const size_t size       = N * sizeof(T);
        static const size_t sz64       = Y_U64_FOR_SIZE(size);

        Y_DECL_ARGS(T,type);

        inline virtual  ~BEpod() throw()
        {
            //memset(data,0,sizeof(data));
        }

        inline BEpod(const_type *addr) throw() :
        data()
        {
            assert(addr);
            memcpy(data,addr,size);
        }

        inline BEpod(const BEpod &other) throw() :
        data()
        {
            memcpy(data,other.data,size);
        }

        inline virtual const void * ro()     const throw() { return data; }
        inline virtual size_t       length() const throw() { return size; }
        
    private:
        uint64_t data[sz64];
        Y_DISABLE_ASSIGN(BEpod);
    };

}

#endif
