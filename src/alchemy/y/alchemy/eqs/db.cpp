
#include "y/alchemy/eqs/db.hpp"
#include "y/string/tokenizer.hpp"

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

            "Ox1:H+:-OxH2:OxH-:@10^(-1.2)",
            "Ox2:H+:-OxH-:Ox--:@10^(-4.3)",

            "Citric1:H+:-CitH3:CitH2-:@10^(-3.13)",
            "Citric2:H+:-CitH2-:CitH--:@10^(-4.76)",
            "Citric3:H+:-CitH--:Cit---:@10^(-6.40)"


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

    }

}
