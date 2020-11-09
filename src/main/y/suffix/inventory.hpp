
//! \file

#ifndef Y_SUFFIX_INVENTORY_INCLUDED
#define Y_SUFFIX_INVENTORY_INCLUDED 1

#include "y/suffix/manifest.hpp"

namespace upsylon
{
    
    //__________________________________________________________________________
    //
    //
    //! simple inventory for accessible<CODE>
    //
    //__________________________________________________________________________
    template <typename CODE, typename BASE_CLASS = suffix_collection>
    class suffix_inventory : public suffix_manifest<CODE,BASE_CLASS>
    {
    public:
        typedef suffix_manifest<CODE,BASE_CLASS> manifest_type;
        
        inline explicit suffix_inventory() : manifest_type() {}
        inline virtual ~suffix_inventory() throw() {}
        
        inline bool insert(const accessible<CODE> &path)
        {
            return this->insert_at(path);
        }
        
        inline bool search(const accessible<CODE> &path) const throw()
        {
            return this->search_at(path);
        }
        
        inline bool remove(const accessible<CODE> &path) throw()
        {
            return this->remove_at(path);
        }
            
        
    
    };
    
    
}

#endif

