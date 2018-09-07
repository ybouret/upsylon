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
        struct Boot
        {
        public:

            //__________________________________________________________________
            //
            //! species+weight
            //__________________________________________________________________
            class Actor : public Object
            {
            public:
                typedef intr_ptr<string,Actor> Pointer;           //!< for database
                explicit Actor( Species &s, const int w) throw(); //!< initialize
                virtual ~Actor() throw();                         //!< destructor
                const string & key() const throw();               //!< sp->name

                const Species::Pointer species; //!< shared species
                const int              weight;  //!< weight

                //! output
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
                typedef arc_ptr<Constraint> Pointer;       //!< for list of constraints
                const double value;                        //!< the constraint value
                explicit Constraint(const double) throw(); //!< initialize
                virtual ~Constraint() throw();             //!< desctructor

                //! add/modify an actor
                Constraint & add( Species &species, const int weight );
                //! add/modify +1
                inline Constraint &add(Species &species) { return add(species,1); }

                //! output
                friend std::ostream & operator<<( std::ostream &os, const Constraint &a);

                //! store into a matrix row
                template <typename T>
                void fill( array<T> &arr ) const
                {
                    for(size_t j=arr.size();j>0;--j) arr[j] = 0;
                    for( Actors::const_iterator i=actors.begin();i!=actors.end();++i)
                    {
                        const Actor   &a = **i;
                        const size_t   j = a.species->indx;
                        arr[j] = static_cast<T>( a.weight );
                    }
                }


            private:
                Actors actors;
                Y_DISABLE_COPY_AND_ASSIGN(Constraint);
            };

            //__________________________________________________________________
            //
            typedef list<Constraint::Pointer> Constraints; //!< database of actors
            //__________________________________________________________________


            //! A list of constraints
            class Loader : public Constraints
            {
            public:
                explicit Loader() throw();
                virtual ~Loader() throw();

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

                //! output
                friend std::ostream & operator<<( std::ostream &os, const Loader &b);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Loader);
            };

        };

    }
}

#endif

