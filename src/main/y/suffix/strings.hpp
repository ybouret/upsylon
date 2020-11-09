
//! \file

#ifndef Y_SUFFIX_STRINGS_INCLUDED
#define Y_SUFFIX_STRINGS_INCLUDED 1

#include "y/suffix/manifest.hpp"

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

        //! buffer insertion
        inline bool insert(const memory::ro_buffer &key) { return this->insert_by(key.as<char>(),key.length()); }

        

        //! text insertion
        inline bool insert(const char *key) { return this->insert_by(key,key?strlen(key):0); }


        //! string search
        inline bool search(const memory::ro_buffer &key) const throw() { return this->search_by(key.as<char>(),key.length()); }

       
        //! text search
        inline bool search(const char *key) const throw()
        {
            return this->search_by(key,key?strlen(key):0);
        }

        //! string removal
        inline bool remove(const memory::ro_buffer &key)  throw() { return this->remove_by(key.as<char>(),key.length()); }

        
        //! text removal
        inline bool remove(const char *key) throw()
        {
            return this->remove_by(key,key?strlen(key):0);
        }

    };



}

#endif
