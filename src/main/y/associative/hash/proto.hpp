

//! \file

#ifndef Y_HASH_PROTO_INCLUDED
#define Y_HASH_PROTO_INCLUDED 1

#include "y/associative/hash/table.hpp"


namespace upsylon
{
    //__________________________________________________________________________
    //
    //! prototype of hash_table based container
    /**
     
     
     */
    //__________________________________________________________________________
    template <
    typename KEY,
    typename T,
    typename NODE,
    typename KEY_HASHER,
    typename BASE_TYPE>
    class hash_proto : public BASE_TYPE
    {
    public:
        //______________________________________________________________________
        //
        // types and declaration
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);
        Y_DECL_ARGS(KEY,key_type);
        typedef NODE      node_type;
        typedef BASE_TYPE base_type;
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~hash_proto() throw() {}
        
        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual size_t size()     const throw() { return table.size();     }
        inline virtual size_t capacity() const throw() { return table.capacity(); }
        inline virtual size_t reserve(const size_t n)  { table.reserve(n);        }
        inline virtual void   free() throw()           { table.free();            }
        inline virtual void   release() throw()        { table.release();         }
        
        
        
    protected:
        //! setup
        inline explicit hash_proto() :
        base_type(),
        table(),
        hash()
        {}
        
        hash_table<NODE>  table; //!< internal table
        
    public:
        mutable KEY_HASHER hash;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_proto);
    };

}

#endif
