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

        class Flow
        {
        public:
            enum State
            {
                Bounded,
                Endless
            };

            static const char *StateText(const State) throw(); //!< BOUNDED / ENDLESS
            const char        *stateText()     const throw(); //!< TypeText(type)

            explicit Flow(const State) throw(); //!< initialize
            virtual ~Flow()            throw(); //!< cleanup

            const State state;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Flow);
        };
        
        //______________________________________________________________________
        //
        //
        //! a simple Equilibrium description
        //
        //______________________________________________________________________
        class Equilibrium : public Labeled, public Flow
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

        protected:
            //! setup
            template <typename NAME> inline
            explicit Equilibrium(const NAME &id) :
            Labeled(id),
            Flow(Endless),
            reac(),
            prod(),
            used()
            {
            }
            
        public:
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
           
            //! get constant value
            virtual double K(double) const = 0;

            //__________________________________________________________________
            //
            // setup methods
            //__________________________________________________________________

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
            
            //__________________________________________________________________
            //
            // query methods
            //__________________________________________________________________

            unit_t        deltaCharge()                  const throw(); //!< charge creation query
            const string &key()                          const throw(); //!< key for set
            string        Kstr(const double t)           const;         //!< get constant as string
            void          verify(const unsigned flags)   const;         //!< verify with given flags
            size_t        countPrimaryReac()             const throw(); //!< with unit rating
            size_t        countPrimaryProd()             const throw(); //!< with unit rating
            unit_t        stoichiometry(const Species &) const throw(); //!< probe used compound

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

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Actors         reac;    //!< reactant
            const Actors         prod;    //!< product
            const Compound::Map  used;    //!< all of'em
            
            //__________________________________________________________________
            //
            // output methods
            //__________________________________________________________________

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
