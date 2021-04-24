

//! \file

#ifndef Y_IOS_CARRIER_NETWORK_INCLUDED
#define Y_IOS_CARRIER_NETWORK_INCLUDED 1


#include "y/ios/carrier.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    
    namespace ios
    {
        
        //______________________________________________________________________
        //
        //
        //! network carrier: fixed size in an heterogeneous environment
        //
        //______________________________________________________________________
        template <typename T>
        class network_carrier : public carrier_of<T>
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
            inline explicit network_carrier() throw() :
            carrier_of<T>(comms::computed_block_size,comms::distributed)
            {
            }
            
            inline virtual ~network_carrier() throw() {}
            
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            inline virtual size_t copy(void *target, const void *source) const
            {
                assert(target);
                assert(source);
                memcpy(target,source,sizeof(T));
                return sizeof(T);
            }
            
            inline virtual size_t save(ios::ostream &fp, const void *source) const
            {
                assert(source);
                fp.write_nbo( *static_cast<const_type*>(source) );
                return sizeof(T);
            }
            
            inline virtual size_t load(void *target, ios::istream &fp) const
            {
                size_t shift = 0;
                if( !fp.query_nbo(*static_cast<mutable_type*>(target),shift) ) this->throw_missing_bytes();
                assert(sizeof(T)==shift);
                return sizeof(T);
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(network_carrier);
        };
    }
    
}

#endif

