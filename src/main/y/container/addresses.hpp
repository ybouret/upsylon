
//! \file

#ifndef Y_ADDRESSES_INCLUDED
#define Y_ADDRESSES_INCLUDED 1

#include "y/os/oor.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    
#define Y_ADDRESSES_PROLOG(N) size(N), addr() { clr(); do
#define Y_ADDRESSES_EPILOG()  while(false); }
#define Y_ADDRESSES(I)        addr[I] = (void*)&a##I
    
    template <typename T>
    class addresses
    {
    public:
        Y_DECL_ARGS(T,type);
        
        static const size_t max_size = 7;
        
        inline ~addresses() throw()
        {
            clr();
            out_of_reach::fill( (void*)&size,0,sizeof(size));
        }
        
        inline addresses(const addresses &other) throw() :
        size(other.size),
        addr()
        {
            out_of_reach::copy(addr,other.addr,sizeof(addr));
        }
        
        inline addresses() throw() : Y_ADDRESSES_PROLOG(0) {
        } Y_ADDRESSES_EPILOG()
        
        inline addresses(const_type &a0) throw() : Y_ADDRESSES_PROLOG(1) {
            Y_ADDRESSES(0);
        } Y_ADDRESSES_EPILOG()
        
        
        inline addresses(const_type &a0,
                         const_type &a1) throw() : Y_ADDRESSES_PROLOG(2) {
            Y_ADDRESSES(0);
            Y_ADDRESSES(1);
        } Y_ADDRESSES_EPILOG()
        
        inline addresses(const_type &a0,
                         const_type &a1,
                         const_type &a2) throw() : Y_ADDRESSES_PROLOG(3) {
            Y_ADDRESSES(0);
            Y_ADDRESSES(1);
            Y_ADDRESSES(2);
        } Y_ADDRESSES_EPILOG()
        
        inline addresses(const_type &a0,
                         const_type &a1,
                         const_type &a2,
                         const_type &a3) throw() : Y_ADDRESSES_PROLOG(4) {
            Y_ADDRESSES(0);
            Y_ADDRESSES(1);
            Y_ADDRESSES(2);
            Y_ADDRESSES(3);
        } Y_ADDRESSES_EPILOG()
        
        
        inline addresses(const_type &a0,
                         const_type &a1,
                         const_type &a2,
                         const_type &a3,
                         const_type &a4) throw() : Y_ADDRESSES_PROLOG(5) {
            Y_ADDRESSES(0);
            Y_ADDRESSES(1);
            Y_ADDRESSES(2);
            Y_ADDRESSES(3);
            Y_ADDRESSES(4);
        } Y_ADDRESSES_EPILOG()
        
        inline type & operator[](const size_t indx) throw()
        {
            assert(indx<size);
            assert(addr[indx]!=NULL);
            return *static_cast<type *>( addr[indx] );
        }
        
        inline const_type & operator[](const size_t indx) const throw()
        {
            assert(indx<size);
            assert(addr[indx]!=NULL);
            return *static_cast<const_type *>( addr[indx] );
        }
        
        const size_t size;
    private:
        void * addr[max_size];
        Y_DISABLE_ASSIGN(addresses);
        inline void clr() throw() { out_of_reach::fill(addr,0,sizeof(addr)); }
        
        
    };
    
}

#endif

