
//! \file

#ifndef Y_AQUA_BOOT_CONSTRAINT_INCLUDED
#define Y_AQUA_BOOT_CONSTRAINT_INCLUDED 1

#include "y/aqua/species.hpp"
#include "y/aqua/types.hpp"
#include "y/core/dnode.hpp"

namespace upsylon {

    namespace Aqua
    {
        //______________________________________________________________________
        //
        //
        //! a linear constraint
        //
        //______________________________________________________________________
        class Constraint : public dnode<Constraint>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef core::list_of_cpp<Constraint> List; //!< alias

            //! an actor
            class Actor : public dnode<Actor>
            {
            public:
                typedef core::list_of_cpp<Actor> List; //!< alias

                explicit Actor(const Species &, const Int) throw();//!< setup
                virtual ~Actor() throw();                          //!< cleanup
                friend std::ostream & operator<<(std::ostream &, const Actor &); //!< display

                const Species &species; //!< persistent species
                const Int      weight;  //!< integer weight

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Actor);
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Constraint(const double) throw();    //!< setup value
            virtual ~Constraint() throw();                //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void add(const Species &, const int);         //!< add an actor
            void fill(addressable<Int> &P) const throw(); //!< fill a row of constraint matrix

            //! display
            friend std::ostream & operator<<(std::ostream &, const Constraint &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            double   value; //!< constraint value

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Constraint);
            Actor::List actors;

        };
    }

}

#endif
