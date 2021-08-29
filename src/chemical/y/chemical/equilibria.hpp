
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
                os << '{' << '\n';
                Strings      raw(eqs->size(),as_capacity);
                const size_t w = eqs.rawStrings(raw);
                for(const ENode *node=eqs->head();node;node=node->next)
                {
                    const Equilibrium &eq = ***node;
                    {
                        const string      &rs = raw[eq.indx];
                        os << ' ' << rs;
                        for(size_t i=rs.size();i<w;++i) os << ' ';
                    }
                    const string ks = eq.Kstr(eqs.tdisp);
                    os << " (" << ks << ")\n";
                }
                os << '}';
                return os;
            }
            
            Equilibrium &create(const string &name,
                                const string &kstr,
                                Lua::VM      &vm);
            
            Equilibrium & operator()(const string &info,
                                     Library      &lib,
                                     Lua::VM      &vm);
            
            Equilibrium & operator()(const char  *info,
                                     Library      &lib,
                                     Lua::VM      &vm);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual const_type& bulk() const throw();
            Equilibrium::Set    edb;
            
            size_t rawStrings(Strings &raw) const;
            
            
        public:
            mutable double      tdisp;
        };
    }
}

#endif

