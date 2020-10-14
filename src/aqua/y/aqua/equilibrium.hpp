//! \file

#ifndef Y_AQUA_EQUILIBRIUM_INCLUDED
#define Y_AQUA_EQUILIBRIUM_INCLUDED 1

#include "y/aqua/extents.hpp"

namespace upsylon {

    namespace Aqua
    {

        //______________________________________________________________________
        //
        //
        //! a generic equilibrium
        //
        //______________________________________________________________________
        class Equilibrium : public Object, public Counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef Component::List      Components; //!< alias
            typedef arc_ptr<Equilibrium> Pointer;    //!< alias

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual ~Equilibrium() throw(); //!< cleanup

            //! insert and reset all species indices
            void operator()(const Species &sp, const int nu);

            //! display
            std::ostream & display(std::ostream &os, const size_t width) const;

            //! display
            friend std::ostream & operator<<(std::ostream &os, const Equilibrium &eq);

            double K(const double t) const; //!< call getK(t), set internal scaling
            void   validate() const;        //!< check validity


            void   fillNu(addressable<Int> &nu) const throw();  //!< fill topology row

            //! compute indicators only
            double computeQ(const double K0, const accessible<double> &C0) const throw();

            //! compute gradient of Q, return Q
            double computeQ(addressable<double> &Phi, const double K0, const accessible<double> &C0) const throw();

            //! try to solve
            /**
             \param K0      precomputed constant
             \param C0      concentrations to move, size=M
             \param arr     [0...M-1] array, helper
             \param verbose for verbosity
             */
            bool   sweep(const double K0, addressable<double> &C0, double arr[], const bool verbose) const;

            //! compute  Ctry = C0 + x * nu, truncated
            void evolve(addressable<double> &Ctry, const accessible<double> &C0, const double x) const throw();


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string     name;        //!< unique name
            const int        dn;          //!< sum nu
            const int        dr;          //!< sum (-(nu<0))
            const int        dp;          //!< sum (nu>0)
            const double     idn;         //!< 1.0/dn if exists
            const Components components;  //!< all components
            const Components reactants;   //!< reactants (set with nu>0)
            const Components products;    //!< products  (set with nu>0)

        protected:
            //! setup
            template <typename ID> inline
            explicit Equilibrium(const ID &id) :
            name(id),
            dn(0),
            dr(0),
            dp(0),
            idn(0),
            components(),
            reactants(),
            products(),
            maxCompSize(0)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);

            size_t     maxCompSize;
            void display_list(std::ostream &os, const Components &l) const;
            virtual double getK(const double t) const = 0;

        };





        //______________________________________________________________________
        //
        //
        //! a constant equilibrium, K(t)==Keq
        //
        //______________________________________________________________________
        class ConstEquilibrium : public Equilibrium
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~ConstEquilibrium() throw(); //!< cleanup

            //! setup by name+value
            template <typename ID> inline
            explicit ConstEquilibrium(const ID &id, const double Kvalue) :
            Equilibrium(id),
            Keq(Kvalue){}

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const double Keq; //!< returned by getK()

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            virtual double getK(const double t) const;

        };

    }

}

#endif

