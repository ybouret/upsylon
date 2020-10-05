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
        class Library; //!< forward declaration

        typedef matrix<int> iMatrix;
        
        class Constraint : public inode<Constraint>
        {
        public:
            typedef core::list_of_cpp<Constraint> List;

            //! an actor
            class Actor : public inode<Actor>
            {
            public:
                typedef core::list_of_cpp<Actor> List;
                explicit Actor(const Species &s,
                               const int      w) throw();
                virtual ~Actor() throw();

                const Species &species;
                const int      weight;

                friend std::ostream & operator<<(std::ostream &os, const Actor &a);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Actor);
            };


            explicit Constraint(const double) throw();
            virtual ~Constraint() throw();

            double      value; //!< constraint value

            void add(const Species &species,
                     const int      weight);
            
            void fill(addressable<int> &P) const throw();

            friend std::ostream & operator<<(std::ostream &os, const Constraint &cc);
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Constraint);
            Actor::List actors;

        };


        class Boot : public Constraint::List
        {
        public:
            explicit Boot() throw();
            virtual ~Boot() throw();

            Constraint & operator()( const double value );

            void electroneutrality(const Library &);
            void conserve(const double, const Species &a);
            void conserve(const double, const Species &a, const Species &b);
            void conserve(const double, const Species &a, const Species &b, const Species &c);

            const iMatrix P;
            const iMatrix S;
            void  fill( addressable<double> &Lambda ) const throw();
            void  quit() throw();
            bool  init(Library &);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boot);
            core::temporary_acquire<2> keep;
        };

    }

}

#endif
