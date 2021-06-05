
//! \file

#ifndef Y_ADDRESSES_INCLUDED
#define Y_ADDRESSES_INCLUDED 1

#include "y/os/oor.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    
#define Y_ADDRESSES_PROLOG(N) size(N), addr() { clr(); do //!< prolog for addresses constructor
#define Y_ADDRESSES_EPILOG()  while(false); }             //!< prolog for addresses constructor
#define Y_ADDRESSES(I)        addr[I] = (void*)&a##I      //!< setup addresses
    
    //__________________________________________________________________________
    //
    //
    //! semi-variadic addresses
    //
    //__________________________________________________________________________
    template <typename T>
    class addresses
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);              //!< aliases
        static const size_t max_size = 7; //!< max size...
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! clean all
        inline ~addresses() throw()
        {
            clr();
            out_of_reach::fill( (void*)&size,0,sizeof(size));
        }
        
        //! no-throw copy
        inline addresses(const addresses &other) throw() :
        size(other.size),
        addr()
        {
            out_of_reach::copy(addr,other.addr,sizeof(addr));
        }
        
        //! setup empty
        inline addresses() throw() : Y_ADDRESSES_PROLOG(0) {
        } Y_ADDRESSES_EPILOG()
        
        //! setup 1 address
        inline addresses(const_type &a0) throw() : Y_ADDRESSES_PROLOG(1) {
            Y_ADDRESSES(0);
        } Y_ADDRESSES_EPILOG()
        
        
        //! setup 2 addresses
        inline addresses(const_type &a0,
                         const_type &a1) throw() : Y_ADDRESSES_PROLOG(2) {
            Y_ADDRESSES(0);
            Y_ADDRESSES(1);
        } Y_ADDRESSES_EPILOG()
        
        //! setup 3 addresses
        inline addresses(const_type &a0,
                         const_type &a1,
                         const_type &a2) throw() : Y_ADDRESSES_PROLOG(3) {
            Y_ADDRESSES(0);
            Y_ADDRESSES(1);
            Y_ADDRESSES(2);
        } Y_ADDRESSES_EPILOG()
        
        //! setup 4 addresses
        inline addresses(const_type &a0,
                         const_type &a1,
                         const_type &a2,
                         const_type &a3) throw() : Y_ADDRESSES_PROLOG(4) {
            Y_ADDRESSES(0);
            Y_ADDRESSES(1);
            Y_ADDRESSES(2);
            Y_ADDRESSES(3);
        } Y_ADDRESSES_EPILOG()
        
        //! setup 5 addresses
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
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! access
        inline type & operator[](const size_t indx) throw()
        {
            assert(indx<size);
            assert(addr[indx]!=NULL);
            return *static_cast<type *>( addr[indx] );
        }
        
        //! const access
        inline const_type & operator[](const size_t indx) const throw()
        {
            assert(indx<size);
            assert(addr[indx]!=NULL);
            return *static_cast<const_type *>( addr[indx] );
        }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t size; //!< stored addresses
    private:
        void * addr[max_size];
        Y_DISABLE_ASSIGN(addresses);
        inline void clr() throw() { out_of_reach::fill(addr,0,sizeof(addr)); }
        
        
    };
    
}

#endif

