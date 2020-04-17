//! \file

#ifndef Y_IOS_CONVEYOR_INCLUDED
#define Y_IOS_CONVEYOR_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"

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
            
            const comm_mode comm; //!< guess communication mode

        protected:
            explicit conveyor(const comm_mode) throw();   //!< filled by derived class
            void     missing_bytes(const string &) const; //!< raise exception
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(conveyor);
        };
        
    }
    
}

#endif

