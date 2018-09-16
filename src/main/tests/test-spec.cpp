//!
#include "y/type/spec.hpp"
#include "y/utest/run.hpp"
#include "y/associative/set.hpp"

using namespace upsylon;

namespace
{
    typedef set<string,type_spec> TypesDB;
    static int count = 0;
    template <typename T>
    void update( TypesDB &db )
    {
        const type_spec ts( typeid(T) );
        std::cerr << "update with '" << ts.key() << "'" << std::endl;
        if(!db.insert(ts))
        {
            std::cerr << "|_already in database" << std::endl;
        }

        ++count;

    }

}

Y_UTEST(spec)
{
    TypesDB db;
    update<uint8_t>(db);
    update<uint16_t>(db);
    update<uint32_t>(db);
    update<uint64_t>(db);

    update<int8_t>(db);
    update<int16_t>(db);
    update<int32_t>(db);
    update<int64_t>(db);

    update<char>(db);
    update<short>(db);
    update<int>(db);
    update<long>(db);

    update<unsigned char>(db);
    update<unsigned short>(db);
    update<unsigned int>(db);
    update<unsigned long>(db);

    std::cerr << std::endl;
    std::cerr << "types db: " << std::endl;
    std::cerr << db << std::endl;
    std::cerr << db.size() << "/" << count << std::endl;
}
Y_UTEST_DONE()

