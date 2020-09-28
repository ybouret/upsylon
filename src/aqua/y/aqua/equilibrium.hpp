
//! \file

#ifndef Y_AQUA_EQUILIBRIUM_INCLUDED
#define Y_AQUA_EQUILIBRIUM_INCLUDED 1

#include "y/aqua/species.hpp"
#include "y/core/list.hpp"
#include "y/core/inode.hpp"

namespace upsylon {

    namespace Aqua
    {

        class Component : public Object, public inode<Component>
        {
        public:
            const Species &sp;
            const int      nu;

            explicit Component(const Species &, const int) throw();
            virtual ~Component() throw();
            Component(const Component &) throw();

            typedef core::list_of_cpp<Component> List;
            
        private:
            Y_DISABLE_ASSIGN(Component);
        };

        class Equilibrium : public Object, public Counted
        {
        public:
            typedef Component::List Components;

            virtual ~Equilibrium() throw();



            const string name;
            const int    d_nu;
            const int    d_nu_r;
            const int    d_nu_p;
            void operator()(const Species &sp, const int nu);

            std::ostream & display(std::ostream &os, const size_t width) const;
            friend std::ostream & operator<<(std::ostream &os, const Equilibrium &eq);

            double K(const double t) const;
            void   validate() const;


        protected:
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


        class ConstEquilibrium : public Equilibrium
        {
        public:
            const double Keq;

            virtual ~ConstEquilibrium() throw();


            template <typename ID> inline
            explicit ConstEquilibrium(const ID &id, const double Kvalue) :
            Equilibrium(id),
            Keq(Kvalue){}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            virtual double getK(const double t) const;

        };

    }

}

#endif

