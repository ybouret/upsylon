
//! \file

#ifndef Y_IOS_CARRIER_PRIMARY_INCLUDED
#define Y_IOS_CARRIER_PRIMARY_INCLUDED 1


#include "y/ios/carrier.hpp"

namespace upsylon
{
    
    namespace ios
    {
        
        //______________________________________________________________________
        //
        //
        //! primary carrier: fixed size in an homogeneous environment
        //
        //______________________________________________________________________
        template <typename T>
        class primary_carrier : public carrier_of<T>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit primary_carrier() :
            carrier_of<T>(comms::computed_block_size, comms::homogeneous)
            {
                assert(sizeof(T)==this->clsid.size);
            }
            
            inline virtual ~primary_carrier() throw() {}
            
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
                fp.output(source,sizeof(T));
                return sizeof(T);
            }
            
            inline virtual size_t load(void *target, ios::istream &fp) const
            {
                if( sizeof(T) != fp.try_query(target,sizeof(T))) this->throw_missing_bytes();
                return sizeof(T);
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(primary_carrier);
        };
    }
    
}

#endif

