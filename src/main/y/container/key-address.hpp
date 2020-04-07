//! \file
#ifndef Y_KEY_ADDRESS_INCLUDED
#define Y_KEY_ADDRESS_INCLUDED 1

#include "y/memory/buffer.hpp"
#include "y/os/static-check.hpp"
#include <cstring>

namespace upsylon {


    namespace core {
        //! sort addresses by increasing values
        void sort_addresses( void **addr, const size_t n) throw();
        //! lexicographics comparison of adresses
        int  lcmp_addresses( void * const *lhs, void * const *rhs, const size_t n) throw();
    }

    //! constructor part
#define Y_KEY_ADDRESS_CTOR() memory::ro_buffer(), addr()

    //! make a key from a given number of addresses
    template <size_t N>
    class key_address : public memory::ro_buffer
    {
    public:
        //! cleanup
        inline virtual ~key_address() throw() { clear(); }


        //! hard copy
        inline          key_address(const key_address &_) throw() : Y_KEY_ADDRESS_CTOR() { memcpy(addr,_.addr,sizeof(addr)); }

        //! buffer interface: ro
        inline const void  *ro()     const throw() { return       &addr[0]; }

        //! buffer interface: length in bytes
        inline size_t       length() const throw() { return sizeof(addr);   }

        //! named 1D setup
        template <typename T>
        inline key_address( const T &arg0 ) throw()
        {
            Y_STATIC_CHECK(N==1,invalid_size);
            clear();
            addr[0] = (void*)&arg0;
        }

        //! named 2D setup
        template <typename T>
        inline key_address( const T &arg0, const T &arg1 ) throw()
        {
            Y_STATIC_CHECK(N==2,invalid_size);
            clear();
            addr[0] = (void*)&arg0;
            addr[1] = (void*)&arg1;
            update();
        }


        //! named 2D setup
        template <typename T, typename U>
        inline key_address( const T &arg0, const U &arg1 ) throw()
        {
            Y_STATIC_CHECK(N==2,invalid_size);
            clear();
            addr[0] = (void*)&arg0;
            addr[1] = (void*)&arg1;
            update();
        }



        //! named 3D setup
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

        //! named 4D setup
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

        //! comparison by lexicographic order
        static inline int compare( const key_address &lhs, const key_address &rhs ) throw()
        {
            return core::lcmp_addresses(lhs.addr, rhs.addr, N);
        }


        //! test by comparison
        friend inline bool operator==( const key_address &lhs, const key_address &rhs ) throw()
        {
            return 0 == compare(lhs,rhs);
        }

        //! test by comparison
        friend inline bool operator!=( const key_address &lhs, const key_address &rhs ) throw()
        {
            return 0 != compare(lhs,rhs);
        }

        //! get addresses
        inline const void * operator[](const size_t i) const throw()
        {
            assert(i<N);
            return addr[i];
        }
        
        //! cast address
        template <typename T>
        const T * as(const size_t i=0) const throw()
        {
            const key_address &self = *this;
            return static_cast<const T *>( self[i] );
        }
        
        //! cast address
        template <typename T>
        T * as(const size_t i=0)   throw()
        {
            const key_address &self = *this;
            return (T*)( self[i] );
        }

    private:
        Y_DISABLE_ASSIGN(key_address);
        void *addr[N];

        inline void clear() throw()  { memset(addr,0,sizeof(addr));  }
        inline void update() throw() { core::sort_addresses(addr,N); }
    };

}

#endif

