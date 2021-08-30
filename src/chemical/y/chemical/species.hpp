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
            typedef intr_ptr<string,const Species>     Pointer; //!< smart pointer
            typedef hash_set<string,Pointer>           Set;     //!< database
            static const char *Status(const size_t) throw();    //!< waiting,leading, seeking
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! setup
            template <typename ID> inline
            Species(const ID &the_name, const unit_t the_charge) :
            Labeled(the_name),
            charge(the_charge),
            rating(0)
            {}
            
            //! cleanup
            virtual ~Species() throw();
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string & key() const throw(); //!< for Set
            
            //! output with internal alignement
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

        typedef Species::Set::node_type SNode;    //!< for iterating

    }
}

#endif

