
//! \file

#ifndef Y_FROZEN_INCLUDED
#define Y_FROZEN_INCLUDED 1

#include "y/type/args.hpp"
#include "y/type/collapse.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! base class to provide frozen with memory
    //
    //__________________________________________________________________________
    class frozen_
    {
    public:
        virtual     ~frozen_() throw();        //!< release memory
        const size_t block_size;               //!< bytes for  data

    protected:
        explicit     frozen_(const size_t sz); //!< acquire memory
        void        *block_addr;               //!< address of data
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(frozen_);
    };
    
    
    //__________________________________________________________________________
    //
    //
    //! frozen default type
    //
    //__________________________________________________________________________
    template <typename T> class frozen : public frozen_
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type); //!< aliases
        
        //______________________________________________________________________
        //
        // cleanup
        //______________________________________________________________________
        inline virtual     ~frozen() throw()                { collapse( *static_cast<mutable_type*>(block_addr) ); }
        
        //______________________________________________________________________
        //
        // constructors
        //______________________________________________________________________
        
        //! default constructor
        inline explicit     frozen() : frozen_( sizeof(type) )
        {
            new (block_addr) mutable_type();
            
        }
        
        //! copy constructor
        inline frozen( const frozen &other ) : frozen_( sizeof(type) )
        {
            new (block_addr) mutable_type( *other );
            
        }
                   
        //! C++ setup with one argument constructor
        template <typename U>
        inline explicit frozen( const U &args ) : frozen_(sizeof(type))
        {
            new (block_addr) mutable_type(args);
        }
        
        //! C++ setup with two arguments constructor
        template <typename U, typename V>
        inline explicit frozen( const U &argU, const V &argV ) : frozen_(sizeof(type))
        {
            new (block_addr) type(argU,argV);
        }
        
        //______________________________________________________________________
        //
        //! access
        //______________________________________________________________________
        inline const_type & operator*() const throw()
        {
            return *static_cast<const_type*>(block_addr);
        }
        
    private:
        Y_DISABLE_ASSIGN(frozen);
        
    };
    
}

#endif

