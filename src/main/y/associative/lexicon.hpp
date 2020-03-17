//! \file

#ifndef Y_LEXICON_INCLUDED
#define Y_LEXICON_INCLUDED 1

#include "y/container/associative.hpp"

namespace upsylon {
    
    //! a lexicon inserts key/args
    template <typename KEY, typename T>
    class lexicon : public associative<KEY,T>
    {
    public:
        Y_DECL_ARGS(T,type);       //!< alias
        Y_DECL_ARGS(KEY,key_type); //!< alias
        
        inline virtual ~lexicon() throw() {}                //!< cleanup
        virtual bool insert(param_key_type,param_type) = 0; //!< false if already exists
        
    protected:
        //! setup
        inline explicit lexicon() throw() : associative<KEY,T>() {}
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(lexicon);
    };
    
}

#endif

