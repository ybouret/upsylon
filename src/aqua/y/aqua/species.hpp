#ifndef Y_AQUA_SPECIES_INCLUDED
#define Y_AQUA_SPECIES_INCLUDED 1

#include "y/string.hpp"
#include "y/memory/groove.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {
 
    namespace Aqua
    {
        typedef object         Object;
        typedef counted        Counted;
        typedef memory::groove Groove;

        class Species : public Object, public Counted, public Groove
        {
        public:
            typedef arc_ptr<Species> Pointer;
            virtual ~Species() throw();

            template <typename ID>
            Species(const ID &id, const int z) :
            Object(), Counted(), Groove(),
            name(id),
            Z(z)
            {
            }

            const string name;
            const int    Z;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
    }
    
}
#endif

