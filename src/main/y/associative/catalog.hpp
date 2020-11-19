
//! \file

#ifndef Y_CATALOG_INCLUDED
#define Y_CATALOG_INCLUDED 1

#include "y/container/associative.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! a catalog insert args (possesing a key() method)
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class catalog : public associative<KEY,T>
    {
    public:
        //______________________________________________________________________
        //
        // types and defintions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);       //!< alias
        Y_DECL_ARGS(KEY,key_type); //!< alias

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline  virtual ~catalog() throw() {}         //!< cleanup
        virtual bool     insert(param_type args) = 0; //!< false if already exists
        
    protected:
        inline explicit catalog() throw() : associative<KEY,T>() {} //! setup
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(catalog);
    };
    
    
}

#endif

