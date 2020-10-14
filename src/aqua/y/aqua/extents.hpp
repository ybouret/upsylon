

//! \file

#ifndef Y_AQUA_EXTENTS_INCLUDED
#define Y_AQUA_EXTENTS_INCLUDED 1

#include "y/aqua/extent.hpp"

namespace upsylon {

    namespace Aqua
    {
        class Equilibrium; //!< forward

        //! forward and reverse extent
        class Extents
        {
        public:
            const Extent forward; //!< for reactants
            const Extent reverse; //!< for products
            const bool   blocked; //!< both are blocked

            //! setup
            Extents(const Equilibrium        &eq,
                    const accessible<double> &C,
                    double                   *A) throw();

            //! copy
            Extents(const Extents &other) throw();

            //! cleanup
            ~Extents() throw();

            //! display
            friend std::ostream & operator<<(std::ostream &, const Extents &x);

            //! cut according to value
            double cut(const double value) const throw();

        private:
            Y_DISABLE_ASSIGN(Extents);
        };


    }

}

#endif

