
//! \file

#ifndef Y_AQUA_EQUILIBRIA_INCLUDED
#define Y_AQUA_EQUILIBRIA_INCLUDED 1

#include "y/aqua/equilibrium.hpp"
#include "y/associative/suffix/tree.hpp"
#include "y/container/matrix.hpp"

namespace upsylon {

    namespace Aqua
    {
        typedef matrix<double> Matrix;

        class Equilibria : public suffix_tree<Equilibrium::Pointer>
        {
        public:
            explicit Equilibria();
            virtual ~Equilibria() throw();

            Equilibrium & operator()( Equilibrium *eq );

            std::ostream & display( std::ostream &) const;
            friend std::ostream & operator<<(std::ostream &, const Equilibria & );

            void fillNu( Matrix &nu ) const throw();
            void validate() const;
            
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
