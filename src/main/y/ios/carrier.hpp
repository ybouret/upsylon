//! \file

#ifndef Y_IOS_CARRIER_INCLUDED
#define Y_IOS_CARRIER_INCLUDED 1


#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/comms.hpp"
#include "y/ptr/counted.hpp"
#include "y/ptr/intr.hpp"
#include "y/type/traits.hpp"
#include <typeinfo>

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
            typedef intr_ptr<std::type_info,carrier> pointer; //!< alias
            
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
            const  string         & which() const;                              //!< type id
            friend std::ostream   & operator<<(std::ostream &, const carrier&); //!< display
            const std::type_info  & key() const throw();                        //!< clsid
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const comms::shipping_style    style;  //!< [computed|flexible]_block_size
            const comms::infrastructure    infra;  //!< [homogeneous|distributed]
            const std::type_info          &clsid;  //!< system type info
            const size_t                   bytes;  //!< sizeof(T)
            
        protected:
            //! setup
            explicit carrier(const comms::shipping_style _style,
                             const comms::infrastructure _infra,
                             const std::type_info       &_clsid,
                             const size_t                _bytes) throw();
            
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
                                       const comms::infrastructure _infra ) throw() :
            carrier(_style,_infra,typeid( typename type_traits<T>::mutable_type ), sizeof(T) )
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(carrier_of);
            
        };
        
    }
}

#endif
