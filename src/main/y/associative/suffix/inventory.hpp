
//! \file

#ifndef Y_SUFFIX_INVENTORY_INCLUDED
#define Y_SUFFIX_INVENTORY_INCLUDED 1

#include "y/associative/suffix/manifest.hpp"

namespace upsylon
{
    
    //__________________________________________________________________________
    //
    //
    //! simple inventory for 'accessible' paths
    //
    //__________________________________________________________________________
    template <typename CODE, typename BASE_CLASS = suffix_collection>
    class suffix_inventory : public suffix_manifest<CODE,BASE_CLASS>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef suffix_manifest<CODE,BASE_CLASS> manifest_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! setup
        inline explicit suffix_inventory() : manifest_type() {}

        //! cleanup
        inline virtual ~suffix_inventory() throw() {}

        //! copy
        inline suffix_inventory(const suffix_inventory &other) :
        collection(), manifest_type(other)
        {
        }

        //! assign using manifest
        inline suffix_inventory & operator=( const suffix_inventory &other)
        {
            manifest_type &self = *this; self = other;
            return *this;
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! insert matching path
        inline bool insert(const accessible<CODE> &path)
        {
            return this->insert_at(path);
        }

        //! search path
        inline bool search(const accessible<CODE> &path) const throw()
        {
            return this->search_at(path);
        }


        //! remove path
        inline bool remove(const accessible<CODE> &path) throw()
        {
            return this->remove_at(path);
        }

    };
    
    
}

#endif

