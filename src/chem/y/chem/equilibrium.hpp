//! \file
#ifndef Y_CHEM_EQUILIBRIUM_INCLUDED
#define Y_CHEM_EQUILIBRIUM_INCLUDED 1

#include "y/chem/species.hpp"
#include "y/sequence/array.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //! Equilibrium description
        class Equilibrium : public Object
        {
        public:
            typedef intr_ptr<string,Equilibrium> Pointer; //!< alias

            //! a component
            class Component : public Object
            {
            public:
                Component              *next;  //!< for list
                Component              *prev;  //!< for list
                const Species::Pointer  sp;    //!< species
                const int               nu;    //!< stoichio
                
                typedef core::list_of_cpp<Component> List; //!< list type

                //! initialize
                explicit Component( Species &s, const int n) throw();
                //! destructor
                virtual ~Component() throw();
                //! compare by index
                static  int Compare( const Component *lhs, const Component *rhs,void*);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Component);
            };

            //! reaction name
            const string name;

            //! the name for Reaction database
            const string & key() const throw();

            //! desctructor
            virtual ~Equilibrium() throw();

            //! list of reactants
            const Component::List  & reactants() const throw();

            //! list of products
            const Component::List  & products()  const throw();

            //! add a new species to the reaction
            void add( Species &sp, const int nu );

            //! add with nu=1
            inline void add( Species &sp ) { add(sp,1);  }
            //! add with nu=-1
            inline void sub( Species &sp ) { add(sp,-1); }


            //! get the constant value
            virtual double  operator()( double t ) const = 0;

            //! output
            friend std::ostream & operator<<( std::ostream &os, const Equilibrium &rxn );

            //! check valid equation
            void validate();

            //! compute Gamma with precomputed K
            double Gamma( const double K,  const array<double> &C ) const throw();

            //! compute Gamma and gradient with precomputed K
            double GammaAndPhi( array<double> &grad, const double K, const array<double> &C) const throw();


        protected:
            //! initialize
            inline Equilibrium(const string &id) : name(id), r_list(), p_list() {}
            //! initialize
            inline Equilibrium(const char   *id) : name(id), r_list(), p_list() {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            Component      *extract( const Species &sp ) throw();
            Component::List r_list;
            Component::List p_list;

        };


        //! a reaction with a constant constant (!)
        class ConstEquilibrium : public Equilibrium
        {
        public:
            //! destructor
            inline virtual ~ConstEquilibrium() throw() {}
            //! initialize
            inline explicit ConstEquilibrium(const string &id, const double _K) : Equilibrium(id), K(_K) { check(); }
            //! initialize
            inline explicit ConstEquilibrium(const char   *id, const double _K) : Equilibrium(id), K(_K) { check(); }

            const double K; //!< positive constant value

            inline virtual double  operator()( double ) const { return K; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            void check() const;
        };

    }
}

#endif

