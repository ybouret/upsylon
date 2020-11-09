
//! \file

#ifndef Y_SUFFIX_STRINGS_INCLUDED
#define Y_SUFFIX_STRINGS_INCLUDED 1

#include "y/suffix/manifest.hpp"
#include "y/string.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! specific manifest for strings
    //
    //__________________________________________________________________________
    template <typename BASE_CLASS=suffix_collection>
    class suffix_strings : public suffix_manifest<char,BASE_CLASS>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef suffix_manifest<char,BASE_CLASS> manifest_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit suffix_strings() : manifest_type() {}                //!< setup
        inline virtual ~suffix_strings() throw()           {}                //!< cleanup
        inline suffix_strings(const suffix_strings &_) : manifest_type(_) {} //!< copy

        //! assign
        inline suffix_strings & operator=( const suffix_strings &_ )
        {
            { manifest_type &self = *this; self = _; }
            return *this;
        }

        //______________________________________________________________________
        //
        // specific methods
        //______________________________________________________________________

        //! string insertion
        inline bool insert(const string &key) { return this->insert_by(*key,key.size()); }

        //! text insertion
        inline bool insert(const char *key, const size_t len)
        {
            assert(!(NULL==key&&len>0));
            return this->insert_by(key,len);
        }

        //! text insertion
        inline bool insert(const char *key) { return this->insert_by(key,length_of(key)); }


        //! string search
        inline bool search(const string &key) const throw() { return this->search_by(*key,key.size()); }

        //! text search
        inline bool search(const char *key, const size_t len) const throw()
        {
            assert(!(NULL==key&&len>0));
            return this->search_by(key,len);
        }

        //! text search
        inline bool search(const char *key) const throw()
        {
            return this->search_by(key,length_of(key));
        }

        //! string removal
        inline bool remove(const string &key)  throw() { return this->remove_by(*key,key.size()); }


        //! text removal
        inline bool remove(const char *key, const size_t len) throw()
        {
            assert(!(NULL==key&&len>0));
            return this->remove_by(key,len);
        }

        //! text removal
        inline bool remove(const char *key) throw()
        {
            return this->remove_by(key,length_of(key));
        }

    };



}

#endif
