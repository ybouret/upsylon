
//! \file

#ifndef Y_AQUA_EXTENT_INCLUDED
#define Y_AQUA_EXTENT_INCLUDED 1

#include "y/aqua/equilibrium/component.hpp"

namespace upsylon {

    namespace Aqua
    {

        //______________________________________________________________________
        //
        //
        //! extent info for an equilibrium
        //
        //______________________________________________________________________
        class Extent
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup from reactants/products (nu>0!) and auxialiary data
            Extent(const accessible<double> &C,
                   const Component::List    &L,
                   double                   *A) throw();
            ~Extent() throw();              //!< cleanup
            Extent(const Extent &) throw(); //!< copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const Extent &); //!< display
            double cut( const double value ) const throw();                   //!< cut guess extent, value>=0

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const bool   limited; //!< is limited ?
            const double maximum; //!< value (>0) if limited
            const bool   blocked; //!< limited at value=0

        private:
            Y_DISABLE_ASSIGN(Extent);
        };

    }

}

#endif

