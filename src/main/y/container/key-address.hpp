//! \file
#ifndef Y_KEY_ADDRESS_INCLUDED
#define Y_KEY_ADDRESS_INCLUDED 1

#include "y/memory/buffer.hpp"
#include "y/os/static-check.hpp"
#include <cstring>

namespace upsylon {


    namespace core {
        void sort_addresses( void **addr, const size_t n) throw();
        int  lcmp_addresses( void * const *lhs, void * const *rhs, const size_t n) throw();
    }

#define Y_KEY_ADDRESS_CTOR() memory::ro_buffer(), addr()

    template <size_t N>
    class key_address : public memory::ro_buffer
    {
    public:
        inline virtual ~key_address() throw() { clear(); }
        inline          key_address()                     throw() : Y_KEY_ADDRESS_CTOR() { clear(); }
        inline          key_address(const key_address &_) throw() : Y_KEY_ADDRESS_CTOR() { memcpy(addr,_.addr,sizeof(addr)); }

        inline const void  *ro()     const throw() { return       &addr[0]; }
        inline size_t       length() const throw() { return sizeof(addr);   }

        template <typename T>
        inline key_address( const T &arg0 ) throw()
        {
            Y_STATIC_CHECK(N==1,invalid_size);
            clear();
            addr[0] = (void*)&arg0;
        }

        template <typename T>
        inline key_address( const T &arg0, const T &arg1 ) throw()
        {
            Y_STATIC_CHECK(N==2,invalid_size);
            clear();
            addr[0] = (void*)&arg0;
            addr[1] = (void*)&arg1;
            update();
        }

        template <typename T>
        inline key_address( const T &arg0, const T &arg1, const T &arg2) throw()
        {
            Y_STATIC_CHECK(N==3,invalid_size);
            clear();
            addr[0] = (void*)&arg0;
            addr[1] = (void*)&arg1;
            addr[2] = (void*)&arg2;
            update();
        }

        template <typename T>
        inline key_address( const T &arg0, const T &arg1, const T &arg2, const T &arg3) throw()
        {
            Y_STATIC_CHECK(N==4,invalid_size);
            clear();
            addr[0] = (void*)&arg0;
            addr[1] = (void*)&arg1;
            addr[2] = (void*)&arg2;
            addr[3] = (void*)&arg3;
            update();
        }


        static inline int compare( const key_address &lhs, const key_address &rhs ) throw()
        {
            return core::lcmp_addresses(lhs.addr, rhs.addr, N);
        }

        friend inline bool operator==( const key_address &lhs, const key_address &rhs ) throw()
        {
            return 0 == compare(lhs,rhs);
        }

        friend inline bool operator!=( const key_address &lhs, const key_address &rhs ) throw()
        {
            return 0 != compare(lhs,rhs);
        }



    private:
        Y_DISABLE_ASSIGN(key_address);
        void *addr[N];

        inline void clear() throw()  { memset(addr,0,sizeof(addr));  }
        inline void update() throw() { core::sort_addresses(addr,N); }
    };

}

#endif

