//! \file
#ifndef Y_CHEM_REACTION_INCLUDED
#define Y_CHEM_REACTION_INCLUDED 1

#include "y/chem/species.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //! Reaction description
        class Reaction : public Object
        {
        public:
            typedef intr_ptr<string,Reaction> Pointer;

            //! a component
            class Component : public Object
            {
            public:
                Component              *next;  //!< for list
                Component              *prev;  //!< for list
                const Species::Pointer  sp;    //!< species
                const int               nu;    //!< stoichio

                typedef core::list_of_cpp<Component> List;

                explicit Component( Species &s, const int n) throw();
                virtual ~Component() throw();
                static  int Compare( const Component *lhs, const Component *rhs,void*);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Component);
            };

            //! reaction name
            const string name;

            //! the name for Reaction database
            const string & key() const throw();

            virtual ~Reaction() throw();

            const Component::List  & reactants() const throw();
            const Component::List  & products()  const throw();

            //! add a new species to the reaction
            void add( Species &sp, const int nu );

            //! get the constant value
            virtual double  operator()( double t ) const = 0;

            friend std::ostream & operator<<( std::ostream &os, const Reaction &rxn );

            //! check valid equation
            void validate();

        protected:
            inline Reaction(const string &id) : name(id), r_list(), p_list() {}
            inline Reaction(const char   *id) : name(id), r_list(), p_list() {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reaction);
            Component      *extract( const Species &sp ) throw();
            Component::List r_list;
            Component::List p_list;

        };


        class ConstReaction : public Reaction
        {
        public:
            inline virtual ~ConstReaction() throw() {}
            inline explicit ConstReaction(const string &id, const double _K) : Reaction(id), K(_K) { check(); }
            inline explicit ConstReaction(const char   *id, const double _K) : Reaction(id), K(_K) { check(); }

            const double K;

            inline virtual double  operator()( double ) const { return K; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstReaction);
            void check() const;
        };

    }
}

#endif

