
//! \file

#ifndef Y_CHEMICAL_SPECIES_INCLUDED
#define Y_CHEMICAL_SPECIES_INCLUDED 1

#include "y/chemical/types.hpp"
#include "y/ptr/ptr.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! information about a (unique) species
        //
        //______________________________________________________________________
        class Species : public Indexed
        {
        public:
        
            template <typename ID> inline
            Species(const ID &the_name, const unit_t the_z) :
            Indexed(),
            name(the_name), z(the_z) {}
            
            
            virtual ~Species() throw();
            
            const string name; //!< the name
            const unit_t z;    //!< algebraic charge

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
    }
}

#endif

