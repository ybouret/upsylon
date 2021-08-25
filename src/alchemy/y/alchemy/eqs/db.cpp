
#include "y/alchemy/eqs/db.hpp"
#include "y/string/tokenizer.hpp"
#include "y/jive/pattern/matching.hpp"

namespace upsylon
{

    Y_SINGLETON_IMPL(Alchemy::EqDB);
    
    namespace Alchemy
    {

        static const char *built_in_eqs[] =
        {
            "water:H+:HO-:@1e-14",
            
            "ethanoic:H+:-EtCOOH:EtCOO-:@10^(-4.18)",
            "ammoniac:H+:-NH4+:NH3:@10^(-9.2)",

            "oxalic1:H+:-OxH2:OxH-:@10^(-1.2)",
            "oxalic2:H+:-OxH-:Ox--:@10^(-4.3)",

            "citric1:H+:-CitH3:CitH2-:@10^(-3.13)",
            "citric2:H+:-CitH2-:CitH--:@10^(-4.76)",
            "citric3:H+:-CitH--:Cit---:@10^(-6.40)",

            "phosphoric1:H+:-H3PO4:H2PO4-:@10^(-2.15)",
            "phosphoric2:H+:-H2PO4-:HPO4--:@10^(-7.20)",
            "phosphoric3:H+:-HPO4--:PO4---:@10^(-12.42)"

        };


        EqDB:: ~EqDB() throw()
        {}

        EqDB:: EqDB() : db()
        {

            for(size_t i=0;i<sizeof(built_in_eqs)/sizeof(built_in_eqs[0]);++i)
            {
                (*this)(built_in_eqs[i]);
            }

        }

        EqDB::const_type & EqDB:: bulk() const throw() { return db; }

        void EqDB:: operator()(const string &info)
        {
            tokenizer<char> tkn(info);
            if(tkn.next_with(':'))
            {
                string name( tkn.token(), tkn.units() );
                name.clean_with(" \t");
                std::cerr << "name='" << name << "'" << std::endl;
                if(!db.insert(name,info)) throw exception("multiple equilibrium '%s'",*name);
            }
            else
            {
                throw exception("missing equilibrium name in info");
            }
        }

        const string & EqDB:: operator[](const string &name) const
        {
            const string *ps = db.search(name);
            if(!ps) throw exception("no equilibrium '%s' in database", *name);

            return *ps;
        }

       void Equilibria:: operator()(const string &name,
                                    Library      &lib,
                                    Lua::VM      &vm)
        {
           static const EqDB &_    = EqDB::instance();
           Strings            keys(_->size(),as_capacity);
           _.find(keys,name);
           for(size_t i=1;i<=keys.size();++i)
           {
               (void) parse( _[keys[i]], lib, vm);
           }

        }

        size_t EqDB:: find(sequence<string> &which,
                           const string     &rx) const
        {
            Jive::Matching  matching(rx);
            size_t          count = 0;
            for(const EqInfo::node_type *node=db.head();node;node=node->next)
            {
                const string &label  = node->ckey;
                if(matching.isExactly(label))
                {
                    which.push_back(label);
                    ++count;
                }
            }
            return count;
        }

    }

}
