//! \file

#ifndef Y_IOS_CARRIER_INCLUDED
#define Y_IOS_CARRIER_INCLUDED 1


#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/comms.hpp"
#include "y/type/traits.hpp"
#include "y/type/rtti.hpp"

namespace upsylon
{
 
    namespace ios
    {
        
        //______________________________________________________________________
        //
        //
        //! base class to load/save data in an abstract way
        //
        //______________________________________________________________________
        class carrier : public counted, public object
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef intr_ptr<rtti,carrier> pointer; //!< alias
            static const char              CLSID[]; //!< "ios::carrier"

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual       ~carrier() throw();                                        //!< cleanup
            virtual size_t copy(void *target, const void *source)         const = 0; //!< direct and local copy
            virtual size_t save(ios::ostream &target, const void *source) const = 0; //!< save for later
            virtual size_t load(void *target, ios::istream &source)       const = 0; //!< reload
            
            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            friend std::ostream   & operator<<(std::ostream &, const carrier&); //!< display
            const rtti            & key() const throw();                        //!< clsid
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const comms::shipping_style    style;  //!< [computed|flexible]_block_size
            const comms::infrastructure    infra;  //!< [homogeneous|distributed]
            const rtti                    &clsid;  //!< system type info

        protected:
            //! setup
            explicit carrier(const comms::shipping_style _style,
                             const comms::infrastructure _infra,
                             const std::type_info       &_clsid);
            
            void throw_missing_bytes() const; //!< throw with name
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(carrier);
           
        };
     
        //______________________________________________________________________
        //
        //
        //! base class to register type and size
        //
        //______________________________________________________________________
        template <typename T>
        class carrier_of : public carrier
        {
        public:
            //! cleanup
            inline virtual ~carrier_of() throw() {}
            
        protected:
            //! setup
            inline explicit carrier_of(const comms::shipping_style _style,
                                       const comms::infrastructure _infra ):
            carrier(_style,_infra,typeid( typename type_traits<T>::mutable_type ) )
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(carrier_of);
            
        };
        
    }
}

#endif
