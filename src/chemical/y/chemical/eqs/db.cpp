

#include "y/chemical/eqs/db.hpp"
#include "y/string/tokenizer.hpp"

namespace upsylon
{
    Y_SINGLETON_IMPL(Chemical::EqDB);
    
    namespace Chemical
    {
        static const char *built_in_eqs[] =
        {
            "water:H+:HO-:=1e-14",
            
            "ethanoic:H+:-EtCOOH:EtCOO-:=10^(-4.18)",
            "ammoniac:H+:-NH4+:NH3:=10^(-9.2)",
            
            "oxalic1:H+:-OxH2:OxH-:=10^(-1.2)",
            "oxalic2:H+:-OxH-:Ox--:=10^(-4.3)",
            
            "citric1:H+:-CitH3:CitH2-:=10^(-3.13)",
            "citric2:H+:-CitH2-:CitH--:=10^(-4.76)",
            "citric3:H+:-CitH--:Cit---:=10^(-6.40)",
            
            "phosphoric1:H+:-H3PO4:H2PO4-:=10^(-2.15)",
            "phosphoric2:H+:-H2PO4-:HPO4--:=10^(-7.20)",
            "phosphoric3:H+:-HPO4--:PO4---:=10^(-12.42)",

            "sulfuric:2H+:-H2SO4:SO4--:=10^(1.1)",

            "iwater:-H+:-HO-:=1e14"


        };
        
        
        EqDB:: ~EqDB() throw()
        {
        }
        
        EqDB:: EqDB() : EqMap()
        {
            for(size_t i=0;i<sizeof(built_in_eqs)/sizeof(built_in_eqs[0]);++i)
            {
                const string _(built_in_eqs[i]);
                decl(_);
            }
        }
        
        void EqDB:: decl(const string &info)
        {
            Strings words;
            tokenizer<char>::split_with(words, info, ':');
            if(words.size()<2) throw exception("%s: invalid <%s>",call_sign,*info);
            
            string &name = words[1];
            name.clean_with(" \t");
            
            if(!insert(name,info))
            {
                throw exception("%s multiple <%s>",call_sign, *name);
            }
            
        }
        
    }
    
}

#include "y/jive/pattern/matching.hpp"

namespace upsylon {
    
    namespace Chemical
    {
        size_t EqDB:: find(sequence<string> &name,
                           sequence<string> &info,
                           const string     &rx) const
        {
            Jive::Matching  matching(rx);
            size_t          count = 0;
            for(const node_type *node=head();node;node=node->next)
            {
                const string &label  = node->ckey;
                if(matching.isExactly(label))
                {
                    name.push_back(label);
                    info.push_back(**node);
                    ++count;
                }
            }
            return count;
        }
        
        void Equilibria:: operator()(const string &rx,
                                     Library      &lib,
                                     Lua::VM      &vm)
        {
            static const EqDB &db     =  EqDB::instance();
            Strings            name(db.size(),as_capacity);
            Strings            info(db.size(),as_capacity);
            
            const size_t       count = db.find(name,info,rx);
            for(size_t i=1;i<=count;++i)
            {
                if(edb.search(name[i])) continue;
                load(info[i],lib,vm);
            }
        }
        
        void Equilibria:: operator()(const char  *rx,
                                     Library      &lib,
                                     Lua::VM      &vm)
        {
            const string _(rx);
            return (*this)(_,lib,vm);
        }
        
    }
}

