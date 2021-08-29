
#include "y/chemical/equilibria.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace Chemical
    {
        const char Equilibria::CLID[] = "Chemical::Equilibria";
        
        Equilibria:: ~Equilibria() throw()
        {
        }
        
        Equilibria:: Equilibria() : edb(), tdisp(0)
        {
        }
        
        Equilibria::const_type &Equilibria:: bulk() const throw()
        {
            return edb;
        }
            
        Equilibrium & Equilibria:: use(Equilibrium *eq)
        {
            Equilibrium::Pointer p(eq);
            if(!edb.insert(p))
            {
                throw exception("%s multiple <%s>", CLID, *(p->name));
            }
            
            // update
            size_t nmax = 0;
            for(const ENode *node=edb.head();node;node=node->next)
            {
                nmax = max_of(nmax,(***node).name.size());
            }
            
            size_t i=0;
            for(const ENode *node=edb.head();node;node=node->next)
            {
                const Equilibrium &my = ***node;
                aliasing::_(my.nmax) = nmax;
                aliasing::_(my.indx) = ++i;
            }
            
            return *p;
        }

    }
}

#include "y/lua++/function.hpp"

namespace upsylon
{
    namespace Chemical
    {
        class LuaEquilibrium : public Equilibrium
        {
        public:
            inline LuaEquilibrium(const string &id,
                                  const string &fn,
                                  Lua::VM      &vm):
            Equilibrium(id),
            call(fn,vm)
            {
            }
            
            inline virtual ~LuaEquilibrium() throw()
            {
            }
            
            virtual double K(double t) const
            {
                return call(t);
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibrium);
            mutable Lua::Function<double> call;
            
        };
        
        Equilibrium & Equilibria::create(const string &name,
                                         const string &kstr,
                                         Lua::VM      &vm)
        {
            static const char fn[] = ".create";
            if(kstr.size()<2) throw exception("%s%s invalid constant string '%s' for <%s>",CLID,fn,*kstr,*name);
            
            const char   ch = kstr[0];
            const string kv(&kstr[1],kstr.size()-1);
            switch(ch)
            {
                case '@': return use( new LuaEquilibrium(name,kv,vm) );
                case '=': return use( new ConstEquilibrium(name,vm->eval<double>(kv)));
                default : break;
            }
            throw exception("%s%s invalid first char '%c' in constant string for <%s>",CLID,fn,ch,*name);
        }
    }
}
