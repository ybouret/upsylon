
//! \file

#ifndef Y_CHEMICAL_EQUILIBRIA_INCLUDED
#define Y_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "y/chemical/equilibrium.hpp"
#include "y/lua++/state.hpp"

namespace upsylon
{
    namespace Chemical
    {

        class Equilibria : public Freezable, public gateway<const Equilibrium::Set>
        {
        public:
            static const char CLID[];
            
            explicit Equilibria();
            virtual ~Equilibria() throw();
            
            //! insert and update
            Equilibrium & use(Equilibrium *);
            
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Equilibria &eqs)
            {
                for(const ENode *node=eqs->head();node;node=node->next)
                {
                    const Equilibrium &eq = ***node;
                    eq.display(os,eqs.tdisp) << '\n';
                }
                return os;
            }
            
            template <typename NAME, typename KSTR>
            Equilibrium & operator()(const NAME &name,
                                     const KSTR &kstr,
                                     Lua::VM    &vm)
            {
                const string _name(name);
                const string _kstr(kstr);
                return create(_name,_kstr,vm);
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual const_type& bulk() const throw();
            Equilibrium::Set    edb;
            
            Equilibrium &create(const string &name,
                                const string &kstr,
                                Lua::VM      &vm);
            
        public:
            mutable double      tdisp;
        };
    }
}

#endif

