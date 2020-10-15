

//! \file

#ifndef Y_AQUA_EXTENTS_INCLUDED
#define Y_AQUA_EXTENTS_INCLUDED 1

#include "y/aqua/extent.hpp"

namespace upsylon {

    namespace Aqua
    {
        class Equilibrium; //!< forward

        //______________________________________________________________________
        //
        //
        //! forward and reverse extent
        //
        //______________________________________________________________________
        class Extents
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const unsigned ForwardLimited = 0x01; //!< alias
            static const unsigned ForwardEndless = 0x02; //!< alias
            static const unsigned ReverseLimited = 0x04; //!< alias
            static const unsigned ReverseEndless = 0x08; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup
            Extents(const Equilibrium        &eq,
                    const accessible<double> &C,
                    double                   *A) throw();

            //! copy
            Extents(const Extents &other) throw();

            //! cleanup
            ~Extents() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display
            friend std::ostream & operator<<(std::ostream &, const Extents &x);

            //! cut according to value
            double cut(const double value) const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Extent   forward; //!< for reactants
            const Extent   reverse; //!< for products
            const bool     blocked; //!< both are blocked
            const unsigned reaches; //!< full status

        private:
            Y_DISABLE_ASSIGN(Extents);
        };


    }

}

#endif

