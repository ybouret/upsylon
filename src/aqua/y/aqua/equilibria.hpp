
//! \file

#ifndef Y_AQUA_EQUILIBRIA_INCLUDED
#define Y_AQUA_EQUILIBRIA_INCLUDED 1

#include "y/aqua/equilibrium.hpp"
#include "y/aqua/types.hpp"
#include "y/suffix/storage.hpp"

namespace upsylon {

    namespace Aqua
    {


        //______________________________________________________________________
        //
        //
        //! a database of equilibria
        //
        //______________________________________________________________________
        class Equilibria : public suffix_storage<Equilibrium::Pointer>
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
            Equilibrium  & operator()( Equilibrium * );                            //!< insert a new equilibrium
            std::ostream & display( std::ostream &) const;                         //!< display
            friend std::ostream & operator<<(std::ostream &, const Equilibria & ); //!< display
            
            //! create a constant equilibrium 'id'
            template <typename ID>
            Equilibrium & constant( const ID &id, const double K )
            {
                Equilibria &self = *this;
                return self( new ConstEquilibrium(id,K) );
            }

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! acid -> base+proton
            template <typename ID>
            Equilibrium &weakAcid(const ID &id, const Species &acid, const Species &base, const Species &proton, const double K)
            {
                Equilibrium &eq = constant(id,K);
                eq(acid,-1);
                eq(base,1);
                eq(proton,1);
                return eq;
            }

            //! solvant -> acid+base
            template <typename ID>
            Equilibrium &solvant(const ID &id, const Species &acid, const Species &base, const double K)
            {
                Equilibrium &eq = constant(id,K);
                eq(acid,1);
                eq(base,1);
                return eq;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t maxNameSize; //!< max size of equilibria's names

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
        };

    }
}

#endif
