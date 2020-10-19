//! \file

#ifndef Y_AQUA_BOOT_INCLUDED
#define Y_AQUA_BOOT_INCLUDED 1

#include "y/aqua/species.hpp"
#include "y/aqua/types.hpp"
#include "y/core/inode.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/vector.hpp"
#include "y/core/temporary-acquire.hpp"

namespace upsylon {

    namespace Aqua
    {
        class Library;    //!< forward declaration
        class Equilibria; //!< forward declaration

        //______________________________________________________________________
        //
        //
        //! a linear constraint
        //
        //______________________________________________________________________
        class Constraint : public inode<Constraint>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef core::list_of_cpp<Constraint> List; //!< alias

            //! an actor
            class Actor : public inode<Actor>
            {
            public:
                typedef core::list_of_cpp<Actor> List; //!< alias

                explicit Actor(const Species &, const int) throw();//!< setup
                virtual ~Actor() throw();                          //!< cleanup
                friend std::ostream & operator<<(std::ostream &, const Actor &); //!< display

                const Species &species; //!< persistent species
                const int      weight;  //!< integer weight

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


        //______________________________________________________________________
        //
        //
        //! booting from constraints
        //
        //______________________________________________________________________
        class Boot : public Constraint::List
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Boot() throw(); //!< setup
            virtual ~Boot() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! create a new contraint
            Constraint & operator()( const double value );

            //! add from library
            void electroneutrality(const Library &);

            //! a=C0
            void conserve(const double, const Species &a);

            //! a+b=C0
            void conserve(const double, const Species &a, const Species &b);

            //! a+b+c=C0
            void conserve(const double, const Species &a, const Species &b, const Species &c);

            //! a+b+c+d=C0
            void conserve(const double, const Species &a, const Species &b, const Species &c, const Species &d);


            void  fill(addressable<double> &) const throw(); //!< fill a vector of Nc constraints
            void  quit() throw();                            //!< reset
            void  init(Library &,const Equilibria &eqs);     //!< buildIndices for library and build matrices

            //! display
            friend std::ostream & operator<<(std::ostream &, const Boot &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const iMatrix R;  //!< constraint matrix
            const Int     d;  //!< determinant of R*R'
            const iMatrix S;  //!< supplementary matrix

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boot);
            core::temporary_acquire<4> keep;
        };

    }

}

#endif
