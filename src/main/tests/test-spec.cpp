//!
#include "y/type/spec.hpp"
#include "y/utest/run.hpp"
#include "y/string/display.hpp"

using namespace upsylon;

#if 0
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
#endif

#define TS(TYPE) do { const type_spec &ts = type_spec::of<TYPE>(); \
string_display::align(std::cerr,#TYPE,32) << " => ";\
string_display::align(std::cerr,ts.name_,32) << " : ";\
std::cerr << std::endl;\
} while(false)

struct Dummy
{
};

Y_UTEST(spec)
{
    //type_specs &sp = type_specs::instance();
    
    TS(uint8_t);
    TS(uint16_t);
    TS(uint32_t);
    TS(uint64_t);

    TS(int8_t);
    TS(int16_t);
    TS(int32_t);
    TS(int64_t);


    TS(char);
    TS(unsigned char);

    TS(short);
    TS(unsigned short);

    TS(int);
    TS(unsigned int);

    TS(long);
    TS(unsigned long);

    TS(long long);
    TS(unsigned long long);

    TS(float);
    TS(double);

    TS(string);
    TS(Dummy);

#if 0
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
#endif
}
Y_UTEST_DONE()

