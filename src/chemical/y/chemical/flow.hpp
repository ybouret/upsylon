//! \file

#ifndef Y_CHEMICAL_FLOW_INCLUDED
#define Y_CHEMICAL_FLOW_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! flow description
        //
        //______________________________________________________________________
        class Flow
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            //! the different states
            enum State
            {
                Bounded, //!< by a component
                Endless  //!< usually by solvent
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Flow(const State) throw(); //!< initialize
            virtual ~Flow()            throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static const char *StateText(const State) throw(); //!< BOUNDED / ENDLESS
            const char        *stateText()     const throw(); //!< TypeText(type)

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const State state;                  //!< current state

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Flow);
        };

    }
}
#endif
