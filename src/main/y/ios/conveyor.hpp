//! \file

#ifndef Y_IOS_CONVEYOR_INCLUDED
#define Y_IOS_CONVEYOR_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/type/mark.hpp"

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
            
            //! direct and local copy
            virtual void copy(void *target, const void *source) const = 0;
            
            //! save for later
            virtual size_t save(ios::ostream &target, const void *source) const = 0;
            
            //! reload
            virtual size_t load(void *target, ios::istream &source) const = 0;
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const comms::manner      comms_manner; //!< guess communication mode
            const std::type_info    &tid;
            
            friend std::ostream & operator<<(std::ostream &, const conveyor &);
            
        protected:
            explicit conveyor(const comms::manner,
                              const std::type_info &) throw(); //!< filled by derived class
            void     missing_bytes() const; //!< raise exception
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(conveyor);
        };
        
    }
    
}

#endif

