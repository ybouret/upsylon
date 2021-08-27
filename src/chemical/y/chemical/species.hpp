
//! \file

#ifndef Y_CHEMICAL_SPECIES_INCLUDED
#define Y_CHEMICAL_SPECIES_INCLUDED 1

#include "y/chemical/labeled.hpp"
#include "y/ptr/intr.hpp"

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
        class Species : public Labeled
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef intr_ptr<string,const Species> Pointer;
            typedef hash_set<string,Pointer>       Set;
            typedef Set::node_type                 Node;
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            template <typename ID> inline
            Species(const ID &the_name, const unit_t the_charge) :
            Labeled(the_name),
            charge(the_charge),
            rating(0)
            {}
            
            
            virtual ~Species() throw();
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Species &sp)
            {
                return sp.pad(os << '[' << sp.name << ']');
            }
            
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t charge;    //!< algebraic charge
            const size_t rating;    //!< times involved in equilibria
       
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
    }
}

#endif

