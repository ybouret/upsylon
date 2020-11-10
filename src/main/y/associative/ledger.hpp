//! \file

#ifndef Y_LEDGER_INCLUDED
#define Y_LEDGER_INCLUDED 1

#include "y/suffix/graph.hpp"
#include "y/os/be-address.hpp"

namespace upsylon {
    



    //! ledger of specific address
    template <typename T>
    class ledger_of : public suffix_graph<char,BEaddress>
    {
    public:
        typedef suffix_graph<char,BEaddress> ledger_type;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit ledger_of() : ledger_type() {}              //!< setup
        inline virtual ~ledger_of() throw()         {}              //!< cleanup
        inline ledger_of(const ledger_of &_) : ledger_type(_) {}    //!< copy
      
        //! assign
        inline ledger_of & operator=( const ledger_of &_ )
        {
            { ledger_type &self = *this; self=_; }
            return *this;
        }

        inline bool insert(const T &args)
        {
            const BEaddress addr = args;
            return this->insert_by( (const char *) & (addr.data), addr.size, args );
        }

        void secure(const T &args)
        {
            (void) insert(args);
        }
        
        //! search
        inline bool search(const T &args) const throw()
        {
            const BEaddress addr = args;
            return this->search_by( (const char *) & (addr.data), addr.size);
        }
        
        //! remove
        inline void remove(const T &args) throw()
        {
            const BEaddress addr = args;
            (void) this->remove_by((const char *) & (addr.data), addr.size);
        }

    };

}

#endif
