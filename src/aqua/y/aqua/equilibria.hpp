
//! \file

#ifndef Y_AQUA_EQUILIBRIA_INCLUDED
#define Y_AQUA_EQUILIBRIA_INCLUDED 1

#include "y/aqua/equilibrium.hpp"
#include "y/associative/suffix/tree.hpp"
#include "y/container/matrix.hpp"

namespace upsylon {

    namespace Aqua
    {
        typedef matrix<int>    iMatrix; //!< alias
        typedef matrix<double> Matrix;  //!< alias

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
            Equilibrium & operator()( Equilibrium * );                             //!< insert a new equilibrium
            std::ostream & display( std::ostream &) const;                         //!< display
            friend std::ostream & operator<<(std::ostream &, const Equilibria & ); //!< display

            //! fill topology matrix
            void fillNu(iMatrix &nu) const throw();

            //! compute all constants
            void computeK(addressable<double> &K, const double t) const;

            //! compute all equilibria indicators
            void computeQ(addressable<double> &Q, const accessible<double> &K, const accessible<double> &C) const throw();

            //! compute Jacobian matrix
            void computePhi(Matrix &Phi, const accessible<double> &K, const accessible<double> &C) const throw();

            //! validate all equilibria
            void validate() const;

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
