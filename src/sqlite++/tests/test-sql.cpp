#include "y/utest/run.hpp"
#include "y/sqlite++/sqlite.hpp"

using namespace upsylon;

Y_UTEST(sql)
{
    SQLite::DataBase db(":memory:");
    
}
Y_UTEST_DONE()

