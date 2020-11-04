
//! \file
#ifndef Y_PREFIX_STORAGE_INCLUDED
#define Y_PREFIX_STORAGE_INCLUDED 1

#include "y/associative/prefix/tree.hpp"
#include "y/container/container.hpp"

namespace upsylon
{
    
    //__________________________________________________________________________
    //
    //
    //! anonymous storage
    //
    //__________________________________________________________________________
    template <typename T>
    class prefix_storage : public container, public prefix_tree<char,T>
    {
    public:
        //______________________________________________________________________
        //
        // types and defintions
        //______________________________________________________________________
        typedef prefix_tree<char,T> tree_type; //!< alias
        Y_DECL_ARGS(T,type);                   //!< aliases

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit prefix_storage() : tree_type() {}
        inline virtual ~prefix_storage() throw() {}
        inline  prefix_storage(const prefix_storage &other) :
        collection(),
        container(),
        tree_type(other) {}
        
        inline prefix_storage & operator=( const prefix_storage &other )
        {
            tree_type &self = *this;
            self            = other;
            return *this;
        }

        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual size_t size()     const throw() { return this->dl.size; }
        inline virtual size_t capacity() const throw() { return this->dl.size + this->dp.size; }
        inline virtual void free()             throw() { this->erase(); }
        inline virtual void release()          throw() { this->ditch(); }
        inline virtual void reserve(const size_t n) { this->gain(n); }

        //______________________________________________________________________
        //
        // insert
        //______________________________________________________________________

        //! generic insertion
        inline bool insert(const void *buf, const size_t len, param_type args)
        {
            assert(!(NULL==buf&&len>0));
            return this->__insert((const char *)buf,len,args);
        }
        
        //! insertion by text
        inline bool insert(const char *text, param_type args)
        {
            return this->__insert(text,tree_type::codelen(text),args);
        }
        
        //! insertion by buffer
        inline bool insert(const memory::ro_buffer &buffer, param_type &args)
        {
            return this->__insert( static_cast<const char *>(buffer.ro()), buffer.length(), args);
        }

        //______________________________________________________________________
        //
        // search
        //______________________________________________________________________
        inline const_type *search(const void *buf, const size_t len) const throw()
        {
            assert(!(NULL==buf&&len>0));
            return this->search_by((const char*)buf,len);
        }
        
        inline const_type *search(const char *text) const throw()
        {
            return this->search_by(text,tree_type::codelen(text));
        }
        
        inline const_type *search(const memory::ro_buffer &buffer) const throw()
        {
            return this->search_by(static_cast<const char *>(buffer.ro()), buffer.length());
        }

        //______________________________________________________________________
        //
        // remove
        //______________________________________________________________________

        inline bool remove(const void *buf, const size_t len)   throw()
        {
            assert(!(NULL==buf&&len>0));
            return this->remove_by((const char*)buf,len);
        }
        
        inline bool remove(const char *text)   throw()
        {
            return this->remove_by(text,tree_type::codelen(text));
        }
        
        inline bool remove(const memory::ro_buffer &buffer)   throw()
        {
            return this->remove_by(static_cast<const char *>(buffer.ro()), buffer.length());
        }
        
        
    };
    
}

#endif

