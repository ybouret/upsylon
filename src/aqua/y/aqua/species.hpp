#ifndef Y_AQUA_SPECIES_INCLUDED
#define Y_AQUA_SPECIES_INCLUDED 1

#include "y/string.hpp"
#include "y/memory/groove.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {
 
    namespace Aqua
    {
        typedef object         Object;   //!< alias for base classes
        typedef counted        Counted;  //!< alias for base classes
        typedef memory::groove Groove;   //!< alias for local storage

        //______________________________________________________________________
        //
        //
        //! mandatory data for a species
        //
        //______________________________________________________________________
        class Species : public Object, public Counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef arc_ptr<Species> Pointer; //!< alias for Library

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Species() throw(); //!< cleanup

            //! setup from an ID and a charge
            template <typename ID>
            Species(const ID &id, const int z) :
            Object(), Counted(),
            name(id),
            Z(z),
            indx(0),
            data()
            {
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name;  //!< global name
            const int    Z;     //!< formal charge
            const size_t indx;  //!< index
            Groove       data;  //!< extra data

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
    }
    
}
#endif

