
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
        //! setup
        inline explicit prefix_storage() : tree_type() {}

        //! cleanup
        inline virtual ~prefix_storage() throw() {}

        //! copy
        inline prefix_storage(const prefix_storage &other) :
        collection(),
        container(),
        tree_type(other) {}

        //! assign
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
        inline virtual size_t size()     const throw() { return this->tell(); }                //!< size
        inline virtual size_t capacity() const throw() { return this->tell() + this->fund(); } //!< capacity
        inline virtual void free()             throw() { this->erase(); }                      //!< keep memory
        inline virtual void release()          throw() { this->ditch(); }                      //!< ditch memory
        inline virtual void reserve(const size_t n)    { this->gain(n); }                      //!< gain extra data nodes

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
        //! search by binary path
        inline const_type *search(const void *buf, const size_t len) const throw()
        {
            assert(!(NULL==buf&&len>0));
            return this->__search((const char*)buf,len);
        }

        //! search by C-style text
        inline const_type *search(const char *text) const throw()
        {
            return this->__search(text,tree_type::codelen(text));
        }

        //! search with memory buffer
        inline const_type *search(const memory::ro_buffer &buffer) const throw()
        {
            return this->__search(static_cast<const char *>(buffer.ro()), buffer.length());
        }

        //______________________________________________________________________
        //
        // remove
        //______________________________________________________________________

        //! remove using binary path
        inline bool remove(const void *buf, const size_t len)   throw()
        {
            assert(!(NULL==buf&&len>0));
            return this->__remove((const char*)buf,len);
        }

        //! remove using C-style text
        inline bool remove(const char *text)   throw()
        {
            return this->__remove(text,tree_type::codelen(text));
        }

        //! remove using memory buffer
        inline bool remove(const memory::ro_buffer &buffer)   throw()
        {
            return this->__remove(static_cast<const char *>(buffer.ro()), buffer.length());
        }
        
        
    };
    
}

#endif

