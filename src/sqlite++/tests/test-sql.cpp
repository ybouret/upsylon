#include "y/utest/run.hpp"
#include "y/sqlite++/sqlite.hpp"

using namespace upsylon;

Y_UTEST(sql)
{
    {
        std::cerr << "Memory DataBase" << std::endl;
        SQLite::DataBase db(":memory:");
    }

    if(argc>1)
    {
        std::cerr << "Open DB R/O" << std::endl;
        SQLite::DB db = new SQLite::DataBase(argv[1],SQLite::OpenReadOnly);
        std::cerr << "Preparing statement" << std::endl;
        SQLite::Statement st( db, "SELECT * FROM tracks;" );
    }
}
Y_UTEST_DONE()

