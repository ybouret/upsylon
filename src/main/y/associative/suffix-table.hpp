//! \file

#ifndef Y_SUFFIX_TABLE_INCLUDED
#define Y_SUFFIX_TABLE_INCLUDED 1

#include "y/associative/suffix-tree.hpp"
#include "y/container/associative.hpp"

namespace upsylon {

    //! suffix table, no key stored
    template <typename KEY,typename T>
    class suffix_table : public container, public suffix_tree<T>
    {
    public:
        Y_DECL_ARGS(T,type);       //!< alias
        Y_DECL_ARGS(KEY,key_type); //!< alias
        
        inline explicit suffix_table() : container(), suffix_tree<T>()
        {
        }

        inline explicit suffix_table(const size_t n, const as_capacity_t &_) :
        container(), suffix_tree<T>(n,_)
        {
        }

        inline suffix_table( const suffix_table &other ) :
        container(), suffix_tree<T>(other)
        {
        }


        inline virtual ~suffix_table() throw()
        {
        }

        //----------------------------------------------------------------------
        //
        // container interface
        //
        //----------------------------------------------------------------------

        //! get size
        inline virtual size_t size() const throw()
        {
            return this->dlist.size;
        }

        //! get capacity
        inline virtual size_t capacity() const throw()
        {
            return this->dlist.size + this->dpool.size;
        }

        //! free
        inline virtual void free() throw()
        {
            this->free_all();
        }

        //! reserve
        inline virtual void reserve(const size_t n)
        {
            this->extra(n);
        }

        //! release all
        inline virtual void release() throw()
        {
            this->release_all();
        }


    private:
        Y_DISABLE_ASSIGN(suffix_table);
    };

}

#endif

