
//! \file

#ifndef Y_AQUA_EQUILIBRIA_INCLUDED
#define Y_AQUA_EQUILIBRIA_INCLUDED 1

#include "y/aqua/equilibrium.hpp"
#include "y/aqua/types.hpp"
#include "y/associative/suffix/tree.hpp"

namespace upsylon {

    namespace Aqua
    {


        //______________________________________________________________________
        //
        //
        //! a database of equilibria
        //
        //______________________________________________________________________
        class Equilibria : public suffix_tree<Equilibrium::Pointer>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Equilibria();         //!< setup
            virtual ~Equilibria() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            Equilibrium  & operator()( Equilibrium * );                             //!< insert a new equilibrium
            std::ostream & display( std::ostream &) const;                         //!< display
            friend std::ostream & operator<<(std::ostream &, const Equilibria & ); //!< display
            
            //! create a constant equilibrium 'id'
            template <typename ID>
            Equilibrium & constant( const ID &id, const double K )
            {
                Equilibria &self = *this;
                return self( new ConstEquilibrium(id,K) );
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            size_t maxNameSize;
        };

    }
}

#endif
