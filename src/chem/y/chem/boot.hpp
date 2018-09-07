//! file
#ifndef Y_CHEM_BOOT_INCLUDED
#define Y_CHEM_BOOT_INCLUDED 1

#include "y/chem/library.hpp"
#include "y/associative/set.hpp"
#include "y/sequence/list.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //! holds a list of algebraic constraints
        class Boot : public Object
        {
        public:

            //__________________________________________________________________
            //
            //! species+weight
            //__________________________________________________________________
            class Actor : public Object
            {
            public:
                typedef intr_ptr<string,Actor> Pointer;
                explicit Actor( Species &s, const int w) throw(); //!< initialize
                virtual ~Actor() throw();            //!< destructor
                const string & key() const throw();  //!< sp->name

                const Species::Pointer species; //!< shared species
                const int              weight;  //!< weight
                friend std::ostream & operator<<( std::ostream &os, const Actor &a);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Actor);
            };

            //__________________________________________________________________
            //
            typedef set<string,Actor::Pointer> Actors; //!< database of actors
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! constraint=value + database
            //__________________________________________________________________
            class Constraint : public Object
            {
            public:
                typedef arc_ptr<Constraint> Pointer;
                const double value;
                explicit Constraint(const double) throw(); //!< initialize
                virtual ~Constraint() throw();             //!< desctructor

                //! add/modify an actor
                Constraint & add( Species &species, const int weight );
                inline Constraint &add(Species &species) { return add(species,1); }

                friend std::ostream & operator<<( std::ostream &os, const Constraint &a);

            private:
                Actors actors;
                Y_DISABLE_COPY_AND_ASSIGN(Constraint);
            };

            //__________________________________________________________________
            //
            typedef list<Constraint::Pointer> Constraints; //!< database of actors
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! initialize a list of constraints
            //__________________________________________________________________
            explicit Boot() throw(); //!< constructor
            virtual ~Boot() throw(); //!< destructor

            //! create a new constraint
            Constraint & create(const double value);
            //! [sp1] = C0
            void         conserve( const double C0, Species &sp1 );
            //! [sp1] + [sp2] = C0
            void         conserve( const double C0, Species &sp1, Species &sp2 );
            //! [sp1] + [sp2] + [sp3] = C0
            void         conserve( const double C0, Species &sp1, Species &sp2, Species &sp3 );
            //! sum([sp].z) = 0
            void         electroneutrality(Library &lib);
            //! sum([sp]) = osm
            void         osmolarity(const double osm, Library &lib);

            friend std::ostream & operator<<( std::ostream &os, const Boot &b);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boot);
            Constraints constraints;
        };

    }
}

#endif

