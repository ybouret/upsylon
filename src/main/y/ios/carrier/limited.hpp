

//! \file

#ifndef Y_IOS_CARRIER_LIMITED_INCLUDED
#define Y_IOS_CARRIER_LIMITED_INCLUDED 1

#include "y/ios/carrier.hpp"
#include "y/type/args.hpp"

namespace upsylon {
    
    namespace ios
    {
        //----------------------------------------------------------------------
        //
        //! build internal conveyor
        //
        //----------------------------------------------------------------------
        template <typename CONVEYOR>
        class limited_carrier_proto
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit limited_carrier_proto() throw() : io() {} //!< setup
            inline virtual ~limited_carrier_proto() throw()        {} //!< cleanup
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const CONVEYOR io; //!< the internal conveyor
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(limited_carrier_proto);
        };
        
        //----------------------------------------------------------------------
        //
        //! aggregate a multiple CONVEYOR
        //
        //----------------------------------------------------------------------
        template <
        const size_t           BLOCKS,
        typename               T,
        template <class> class CARRIER
        >
        class limited_carrier : public limited_carrier_proto< CARRIER<T> >, public carrier
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef limited_carrier_proto< CARRIER<T> > base_type;   //!< alias
            static  const size_t block_size = BLOCKS * sizeof(T);    //!< alias
                                                                 
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            inline virtual ~limited_carrier() throw()
            {
            }
            
            //! setup
            inline explicit limited_carrier(const std::type_info &_) throw() :
            base_type(),
            carrier(this->io.style,this->io.infra,_,block_size)
            {
            }
            
            //__________________________________________________________________
            //
            // conveyor interface
            //__________________________________________________________________
            
            //! copy BLOCKS types
            inline virtual size_t copy(void *target, const void *source) const
            {
                char       *p = static_cast<char       *>(target);
                const char *q = static_cast<const char *>(source);
                for(size_t i=BLOCKS;i>0;--i)
                {
                    this->io.copy(p,q);
                    p += sizeof(T);
                    q += sizeof(T);
                }
                return block_size;
            }
            
            //! save BLOCKS types
            inline virtual size_t save(ios::ostream &target, const void *source) const
            {
                const char *q     = static_cast<const char *>(source);
                size_t      total = 0;
                for(size_t i=BLOCKS;i>0;--i)
                {
                    total += this->io.save(target,q);
                    q += sizeof(T);
                }
                return total;
            }
            
            //! load BLOCKS types
            inline virtual size_t load(void *target, ios::istream &source) const
            {
                char  *p     = static_cast<char *>(target);
                size_t total = 0;
                for(size_t i=BLOCKS;i>0;--i)
                {
                    total += this->io.load(p,source);
                    p += sizeof(T);
                }
                return total;
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(limited_carrier);
        };
        
    }
    
}

#endif
