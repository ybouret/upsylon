
//! \file

#ifndef Y_CHEMICAL_COMPOUND_INCLUDED
#define Y_CHEMICAL_COMPOUND_INCLUDED 1

#include "y/chemical/actor.hpp"
#include "y/type/authority.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! Tier for an actor
        //
        //______________________________________________________________________
        enum Tier
        {
            Reactant, //!< is a reactant
            Product   //!< is a product
        };

        //______________________________________________________________________
        //
        //
        //! wrapping an actor with its role
        //
        //______________________________________________________________________
        class Compound : public Object, public authority<const Actor>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const Compound>    Pointer; //!< alias
            typedef suffix_map<be_key,Pointer> Map;     //!< alias


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit      Compound(const Actor &, const Tier) throw(); //!< setup
            virtual      ~Compound() throw();                          //!< cleanup

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Tier tier; //!< retrieve the category


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compound);
        };

        //______________________________________________________________________
        //
        //! iterator on compound
        //______________________________________________________________________
        typedef Compound::Map::data_node CNode;

    }

}

#endif

