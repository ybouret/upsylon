


//! \file

#ifndef Y_IOS_CARRIER_DERIVED_INCLUDED
#define Y_IOS_CARRIER_DERIVED_INCLUDED 1


#include "y/ios/carrier.hpp"
#include "y/type/args.hpp"
#include "y/string.hpp"

namespace upsylon
{
    
    namespace ios
    {
        
        //______________________________________________________________________
        //
        //
        //! derived carrier: flexible size in an heterogeneous environment
        /**
         use to carry serializable types
         */
        //
        //______________________________________________________________________
        template <typename T>
        class derived_carrier : public carrier_of<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type); //!< aliases
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! setup
            inline explicit derived_carrier() :
            carrier_of<T>(comms::flexible_block_size,comms::distributed),
            id( this->clsid.text() )
            {
            }
            
            inline virtual ~derived_carrier() throw() {}
            
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            
            //! use C++ api
            inline virtual size_t copy(void *target, const void *source) const
            {
                assert(target);
                assert(source);
                *static_cast<mutable_type *>(target) = *static_cast<const_type *>(source);
                return sizeof(T);
            }
            
            //! user serializable interface
            inline virtual size_t save(ios::ostream &fp, const void *source) const
            {
                assert(source);
                return static_cast<const_type*>(source)->serialize(fp);
            }
            
            inline virtual size_t load(void *target, ios::istream &fp) const
            {
                size_t             shift = 0;
                const_type         temp  = mutable_type::read(fp,shift,id);
                *static_cast<mutable_type *>(target)  = temp;
                return shift;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(derived_carrier);
            const char *id;
        };
    }
    
}

#endif

