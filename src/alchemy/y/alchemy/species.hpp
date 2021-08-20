
//! \file

#ifndef Y_ALCHEMY_SPECIES_INCLUDED
#define Y_ALCHEMY_SPECIES_INCLUDED 1

#include "y/alchemy/types.hpp"
#include "y/associative/hash/set.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace randomized
    {
        class bits; //!< forward declaration
    }


    namespace Alchemy
    {


        //______________________________________________________________________
        //
        //
        //! one species with minimal info
        //
        //______________________________________________________________________
        class Species :  public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef intr_ptr<string,Species>          Pointer;         //!< alias
            typedef hash_set<string,Species::Pointer> Set;             //!< alias
            typedef Set::node_type                    Node;            //!< alias
            typedef vector<const Pointer,Allocator>   Array;           //!< alias
            static  const int                         min_exp10 = -14; //!< for random concentration
            static  const int                         max_exp10 = 0;   //!< for random concentration
            static  double Concentration(randomized::bits&) throw();   //!< get a random concentration
            
            //__________________________________________________________________
            //
            // C++
            //________________________________________________________________

            //! constructor
            template <typename ID> inline
            explicit Species(const ID    &_name,
                             const unit_t _z,
                             const size_t _indx) :
            name(_name),
            z(_z),
            indx(_indx),
            active(false),
            rating(0)
            {
                checkIndex();
            }

            //! cleanup
            virtual ~Species() throw();

            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string & key() const throw(); //!< for library

            
            //! display as concentration
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Species &sp)
            {
                return (os << '[' << sp.name << ']');
            }

            //! human readible rating
            /**
             - neutral : rating = 0
             - leading : rating = 1
             - seeking : rating > 1
             */
            const char *ratingText() const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name;   //!< identifier
            const unit_t z;      //!< algebraic charge
            const size_t indx;   //!< index in library
            const bool   active; //!< modified upon equilibrium
            const size_t rating; //!< number of use

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
            void checkIndex() const;
        };
        
    }
    
}

#endif

