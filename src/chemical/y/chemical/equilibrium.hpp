//! \file

#ifndef Y_CHEMICAL_EQUILIBRIUM_INCLUDED
#define Y_CHEMICAL_EQUILIBRIUM_INCLUDED 1

#include "y/chemical/actors.hpp"
#include "y/chemical/library.hpp"

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
            // types
            //__________________________________________________________________
            typedef intr_ptr<string,Equilibrium> Pointer; //!< alias
            typedef hash_set<string,Pointer>     Set;     //!< database alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Equilibrium() throw(); //!< cleanup
            
        protected:
            //! setup
            template <typename NAME> inline
            explicit Equilibrium(const NAME &id) :
            Labeled(id),
            reac(),
            prod()
            {
            }
            
        public:
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
           
            //! get constant value
            virtual double K(double) const = 0;
           
            //! add an actor
            void operator()(const unit_t nu, const Species &sp);
            
            //! with parsing
            template <typename RX> inline
            void operator()(const RX &rx, Library &lib)
            {
                Jive::Source source( Jive::Module::OpenData(rx) );
                const Species *ps = 0;
                const unit_t   nu = lib.get(source, &ps); assert(ps!=NULL);
                (*this)(nu,*ps);
            }
            
            //! aligned output
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Equilibrium &eq)
            {
                return eq.pad(os << '<' << eq.name << '>');
            }
            
            //! full display
            template <typename OSTREAM> inline
            OSTREAM & display_raw(OSTREAM &os) const
            {
                return os << (*this)  << " : " << reac << " <=> " << prod;
            }
            
            //! convert raw to sring
            string toRawString() const;
            
            //! full display
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os, const double t=0) const
            {
                 return display_raw(os) << " (" << Kstr(t) << ")";
            }
            
            //! charge creation
            unit_t deltaCharge() const throw();
            
            //! key for set
            const string &key() const throw();
          
            //! get constant as string
            string Kstr(const double t) const;

            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Actors reac; //!< reactant
            const Actors prod; //!< product
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
        };
        
        //______________________________________________________________________
        //
        //! iterator for equilibria
        //______________________________________________________________________
        typedef Equilibrium::Set::node_type ENode;

        //______________________________________________________________________
        //
        //
        //! a simple constant equilibrium
        //
        //______________________________________________________________________
        class ConstEquilibrium : public Equilibrium
        {
        public:
            //! cleanup
            virtual ~ConstEquilibrium() throw();
            
            //! setup
            template <typename NAME> inline
            explicit ConstEquilibrium(const NAME &id, const double Kvalue) :
            Equilibrium(id), K_(Kvalue)
            {
            }
            
            //! constant value
            virtual double K(double) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            const double K_;
        };
        
    }
    
}

#endif
