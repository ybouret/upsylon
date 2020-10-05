//! \file

#ifndef Y_AQUA_BOOT_INCLUDED
#define Y_AQUA_BOOT_INCLUDED 1

#include "y/aqua/species.hpp"
#include "y/core/inode.hpp"
#include "y/container/matrix.hpp"
#include "y/sequence/vector.hpp"
#include "y/core/temporary-acquire.hpp"

namespace upsylon {

    namespace Aqua
    {
        class Library;              //!< forward declaration
        typedef matrix<int> iMatrix;//!< forward declaration

        //! a linear constraint
        class Constraint : public inode<Constraint>
        {
        public:
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


            explicit Constraint(const double) throw();    //!< setup value
            virtual ~Constraint() throw();                //!< cleanup
            void add(const Species &, const int);         //!< add an actor
            void fill(addressable<int> &P) const throw(); //!< fill a row of constraint matrix
            friend std::ostream & operator<<(std::ostream &, const Constraint &); //!< display

            
            double   value;                            //!< constraint value

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Constraint);
            Actor::List actors;

        };

        //! booting from constraints
        class Boot : public Constraint::List
        {
        public:
            explicit Boot() throw(); //!< setup
            virtual ~Boot() throw(); //!< cleanup

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

            const iMatrix P; //!< constraint matrix
            const iMatrix S; //!< supplementary matrix

            //! fill a vector of Nc constraints
            void  fill(addressable<double> &) const throw();
            void  quit() throw();  //!< reset
            bool  init(Library &); //!< buildIndices for library and build matrices
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boot);
            core::temporary_acquire<2> keep;
        };

    }

}

#endif
