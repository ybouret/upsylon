
//! \file

#ifndef Y_AQUA_COMPONENT_INCLUDED
#define Y_AQUA_COMPONENT_INCLUDED 1

#include "y/aqua/species.hpp"
#include "y/aqua/types.hpp"
#include "y/core/list.hpp"
#include "y/core/dnode.hpp"

namespace upsylon {

    namespace Aqua
    {

        //______________________________________________________________________
        //
        //
        //! a species with its coefficient
        //
        //______________________________________________________________________
        class Component : public Object, public dnode<Component>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef core::list_of_cpp<Component> List; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Component(const Species &, const int) throw(); //!< setup
            virtual ~Component() throw();                           //!< cleanup
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Species &sp;  //!< species
            const int      nu;  //!< coef
            const size_t   p;   //!< |nu|
            const size_t   pm1; //!< p-1

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Component);
        };

    }

}

#endif

