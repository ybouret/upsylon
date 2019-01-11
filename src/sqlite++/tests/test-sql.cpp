#include "y/utest/run.hpp"
#include "y/sqlite++/sqlite.hpp"

using namespace upsylon;

namespace
{
    static inline
    int echo(void *,int nc,char **values,char **)
    {
        //std::cerr << "#cols=" << nc << std::endl;
        for(int i=0;i<nc;++i)
        {
            const char *value = values[i];
            if(value!=NULL)
            {
                std::cerr << value << "|";
            }
        }
        std::cerr << std::endl;
        return 0;
    }
}
Y_UTEST(sql)
{
    {
        std::cerr << "Memory DataBase" << std::endl;
        SQLite::DataBase db(":memory:");
    }

    if(argc>1)
    {
        std::cerr << "Open DB R/O" << std::endl;
        SQLite::DataBase db(argv[1],SQLite::OpenReadOnly);
        db.execute("SELECT * FROM tracks;",echo,NULL);

    }
}
Y_UTEST_DONE()

