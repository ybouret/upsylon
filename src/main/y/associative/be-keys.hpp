
//! \file

#ifndef Y_BE_KEYS_INCLUDED
#define Y_BE_KEYS_INCLUDED 1

#include "y/os/be-address.hpp"
#include "y/memory/buffer.hpp"
#include "y/os/static-check.hpp"
#include <cstring>


namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! Big Endian keys from addresses
    //
    //__________________________________________________________________________
    template <size_t N>
    class be_keys : public memory::ro_buffer
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! cleanup
        inline virtual ~be_keys() throw() { zero(); }
        //! default
        inline          be_keys() throw() : addr() { zero(); }
        //! copy
        inline          be_keys(const be_keys &_) throw() : addr()
        {
            memcpy(addr,_.addr,sizeof(addr));
        }
        //! assign
        inline be_keys & operator=(const be_keys &_) throw()
        {
            if(this != &_) memove(addr,_.addr,sizeof(addr));
            return *this;
        }

        //! N=1 setup
        template <typename T> inline
        be_keys(const T &t) throw() : addr()
        {
            Y_STATIC_CHECK(N==1,invalid_size);
            zero();
            addr[0]=(void*)&t;
            make();
        }

        //! N=2 setup
        template <typename T, typename U> inline
        be_keys(const T &t, const U &u) throw() : addr()
        {
            Y_STATIC_CHECK(N==2,invalid_size);
            zero();
            addr[0]=(void*)&t;
            addr[1]=(void*)&u;
            make();
        }


        //! ro_buffer access
        virtual const void * ro()     const throw() { return     &addr[0]; }

        //! ro_buffer length
        virtual size_t       length() const throw() { return sizeof(addr); }

    private:
        void *addr[N];
        inline void zero()  throw() { memset(addr,0,sizeof(addr)); }
        inline void make() throw()
        {
            for(size_t i=0;i<N;++i) addr[i] = BEaddress::BE(addr[i]);
        }

    };
}

#endif
