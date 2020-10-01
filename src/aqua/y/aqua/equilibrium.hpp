//! \file

#ifndef Y_AQUA_EQUILIBRIUM_INCLUDED
#define Y_AQUA_EQUILIBRIUM_INCLUDED 1

#include "y/aqua/species.hpp"
#include "y/core/list.hpp"
#include "y/core/inode.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon {

    namespace Aqua
    {

        //______________________________________________________________________
        //
        //
        //! a species with its coefficient
        //
        //______________________________________________________________________
        class Component : public Object, public inode<Component>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef core::list_of_cpp<Component> List; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Component(const Species &, const int) throw(); //!< setup
            virtual ~Component() throw();                           //!< cleanup
            Component(const Component &) throw();                   //!< copy

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Species &sp;  //!< species
            const int      nu;  //!< coef
            const size_t   p;   //!< |nu|
            const size_t   pm1; //!< p-1

        private:
            Y_DISABLE_ASSIGN(Component);
        };

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

            double K(const double t) const; //!< call getK(t)
            void   validate() const;        //!< check validity


            void   fillNu(addressable<int> &nu) const throw();  //!< fill topology row

            //! compute indicators only
            double computeQ(const double K0, const accessible<double> &C0) const throw();

            //! compute gradient of Q, return Q
            double computeQ(addressable<double> &Phi, const double K0, const accessible<double> &C0) const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name;     //!< unique name
            const int    d_nu;     //!< sum nu
            const int    d_nu_r;   //!< sum (-(nu<0))
            const int    d_nu_p;   //!< sum (nu>0)

        protected:
            //! setup
            template <typename ID> inline
            explicit Equilibrium(const ID &id) :
            name(id),
            d_nu(0),
            d_nu_r(0),
            d_nu_p(0),
            components(),
            reactants(),
            products(),
            maxCompSize(0)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            Components components;
            Components reactants;
            Components products;
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

