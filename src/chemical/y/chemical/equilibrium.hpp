//! \file

#ifndef Y_CHEMICAL_EQUILIBRIUM_INCLUDED
#define Y_CHEMICAL_EQUILIBRIUM_INCLUDED 1

#include "y/chemical/actors.hpp"
#include "y/chemical/library.hpp"
#include "y/chemical/compound.hpp"

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
            // types and definitions
            //__________________________________________________________________
            typedef intr_ptr<string,Equilibrium> Pointer;        //!< alias
            typedef hash_set<string,Pointer>     Set;            //!< database alias
            static const unsigned                Default = 0x00; //!< check charge conservation
            static const unsigned                Minimal = 0x01; //!< check coprimality
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Equilibrium() throw();     //!< cleanup
            bool     isBounded() const throw(); //!< has reactant(s) AND product(s)

        protected:
            //! setup
            template <typename NAME> inline
            explicit Equilibrium(const NAME &id) :
            Labeled(id),
            reac(),
            prod(),
            used()
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

            //! fill topology
            template <typename T> inline
            void fill(addressable<T> &Nu) const throw()
            {
                for(const ANode *node=reac->head();node;node=node->next)
                {
                    const Actor &a = **node;
                    Nu[a.sp.indx] = static_cast<T>(-a.snu);
                }
                
                for(const ANode *node=prod->head();node;node=node->next)
                {
                    const Actor &a = **node;
                    Nu[a.sp.indx] = static_cast<T>(a.snu);
                }
                
            }
            

            void   verify(const unsigned flags) const;           //!< verify with given flags
            size_t countPrimaryReac()   const throw();           //!< with unit rating
            size_t countPrimaryProd()   const throw();           //!< with unit ration
            unit_t stoichiometry(const Species &) const throw(); //!< probe used compound

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Actors         reac; //!< reactant
            const Actors         prod; //!< product
            const Compound::Map  used; //!< all of'em
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            static size_t CountPrimary(const Actors &) throw();
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
