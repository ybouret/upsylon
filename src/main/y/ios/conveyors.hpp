
//! \file

#ifndef Y_IOS_CONVEYORS_INCLUDED
#define Y_IOS_CONVEYORS_INCLUDED 1

#include "y/ios/conveyor.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {
    
    namespace ios
    {
        
        //! database of conveyors
        class conveyors : public singleton<conveyors>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const conveyor>  convoy;          //!< internal shared conveyor
            static  const at_exit::longevity life_time = 0;   //!< for singleton
            
            template <typename T>
            const conveyor & create_for(const comm_environment &terrain);
            
        private:
            virtual ~conveyors() throw();
            explicit conveyors();
            Y_DISABLE_COPY_AND_ASSIGN(conveyors);
            friend class singleton<conveyors>;
            
            const conveyor & insert(const std::type_info &, const convoy &);
            const conveyor * search(const std::type_info &) const throw();
        };
        
    }
    
}

#endif

