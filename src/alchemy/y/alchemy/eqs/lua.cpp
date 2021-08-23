
#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"
#include "y/lua++/state.hpp"

#include "y/string/tokenizer.hpp"
#include "y/sequence/list.hpp"

namespace upsylon
{
    
    namespace Alchemy
    {
        Equilibrium &Equilibria::parse(const char  *info,
                                       Library      &lib,
                                       Lua::State   &vm)
        {
            const string _(info);
            return parse(_,lib,vm);
        }
        
        Equilibrium &Equilibria::parse(const string &info,
                                       Library      &lib,
                                       Lua::State   &vm)
        {
            static const char fn[] = ".parse";
            list<string> words;
            tokenizer<char>::split_with(words,info,':');
            
            if(words.size()<3)
            {
                throw exception("%s%s need at least name:actor:@value",CLID,fn);
            }
            
            for(list<string>::iterator it=words.begin();it!=words.end();++it)
            {
                it->clean_with(" \t");
            }
            
            const string name = words.front(); words.pop_front();
            string       kval = words.back();  words.pop_back();
            if(kval.size()<=1) throw exception("%s%s invalid kval '%s'",CLID,fn,*kval);
            if('@'!=kval[0])   throw exception("%s%s constant '%s' should start with '@'",CLID,fn,*kval);
            
            kval.skip(1);
            const double K  = vm.eval<double>(kval);
            Equilibrium &eq = (*this)(name,K);
            for(list<string>::iterator it=words.begin();it!=words.end();++it)
            {
                const string &a = *it;
                eq.parse(a,lib);
            }
            return eq;
        }
    }
    
}

