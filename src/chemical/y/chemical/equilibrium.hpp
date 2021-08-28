

//! \file

#ifndef Y_CHEMICAL_EQUILIBRIUM_INCLUDED
#define Y_CHEMICAL_EQUILIBRIUM_INCLUDED 1

#include "y/chemical/actors.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! a simple Equilibrium description
        //
        //______________________________________________________________________
        class Equilibrium : public Labeled
        {
        public:
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Equilibrium() throw();
            
            template <typename NAME> inline
            explicit Equilibrium(const NAME &id) :
            Labeled(id),
            reac(),
            prod()
            {
            }
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
           
            //! get constant value
            virtual double K(double) const throw() = 0;
           
            //! add an actor
            void operator()(const unit_t nu, const Species &sp);
            
            //! with parsing
            
            //! aligned output
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Equilibrium &eq)
            {
                return eq.pad(os << '<' << eq.name << '>');
            }
            
            //! full display
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os, const double t=0) const
            {
                return os << (*this)  << " : " << reac << " <=> " << prod << " (" << Kstr(t) << ")";
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Actors reac;
            const Actors prod;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            string Kstr(const double t) const;
        };
        
        class ConstEquilibrium : public Equilibrium
        {
        public:
            virtual ~ConstEquilibrium() throw();
            
            template <typename NAME> inline
            explicit ConstEquilibrium(const NAME &id, const double Kvalue) :
            Equilibrium(id), K_(Kvalue)
            {
            }
            
            virtual double K(double) const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            const double K_;
        };
        
    }
    
}

#endif
