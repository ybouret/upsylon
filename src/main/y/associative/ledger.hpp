//! \file

#ifndef Y_LEDGER_INCLUDED
#define Y_LEDGER_INCLUDED 1

#include "y/associative/suffix/tree.hpp"
#include "y/os/be-address.hpp"

namespace upsylon {
    
    
    //! a ledger of addresses
    class ledger : public suffix_tree<BEaddress>
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        typedef BEaddress              addr_type; //!< any address
        typedef suffix_tree<addr_type> base_type; //!< alias
         
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~ledger() throw();              //!< cleanup
        explicit ledger();                      //!< setup
        ledger(const ledger &);                 //!< copy
        ledger & operator=(const ledger &);    //!< copy/swap
        
    };
    
    //! ledger of specific address
    template <typename T>
    class ledger_of : public ledger
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit ledger_of() : ledger() {}              //!< setup
        inline virtual ~ledger_of() throw()    {}              //!< cleanup
        inline ledger_of(const ledger_of &_) : ledger(_) {}    //!< copy
      
        //! assign
        inline ledger_of & operator=( const ledger_of &_ )
        {
            { ledger &self = *this; self=_; }
            return *this;
        }
        
        //! insert
        inline bool insert(const T &args)
        {
            const addr_type addr = args;
            return this->insert_by( &addr.data,addr.size,addr);
        }
        
        //! search
        inline bool search(const T &args) const throw()
        {
            const addr_type addr = args;
            return NULL != this->search_by( &addr.data,addr.size);
        }
        
        //! secure
        inline void secure(const T &args)
        {
            while( !insert(args) )
                ;
        }
        
        //! remove
        inline void remove(const T &args) throw()
        {
            const addr_type addr = args;
            while( this->remove_by( &addr.data, addr.size) )
                ;
        }
        
    };
    
}

#endif
