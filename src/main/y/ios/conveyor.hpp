//! \file

#ifndef Y_IOS_CONVEYOR_INCLUDED
#define Y_IOS_CONVEYOR_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/type/mark.hpp"
#include "y/ios/comms.hpp"

namespace upsylon {
 
    namespace ios
    {
        
        //! abstract function to convey data
        class conveyor : public counted_object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            virtual ~conveyor() throw();
            
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            
            virtual void   copy(void *target, const void *source)         const = 0; //!< direct and local copy
            virtual size_t save(ios::ostream &target, const void *source) const = 0; //!< save for later
            virtual size_t load(void *target, ios::istream &source)       const = 0; //!< reload
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const comms::delivery    mode; //!< [computed|flexible]_block_size
            const std::type_info    &uuid; //!< system type info
            const size_t             move; //!< sizeof(T)
            
            //! display information
            friend std::ostream & operator<<(std::ostream &, const conveyor &);
            
        protected:
            explicit conveyor(const comms::delivery, const std::type_info &, const size_t) throw(); //!< filled by derived class
            void     missing_bytes() const; //!< raise exception
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(conveyor);
        };
        
    }
    
}

#endif

