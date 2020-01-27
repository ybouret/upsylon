#include "y/type/spec.hpp"
#include "y/utest/run.hpp"
#include "y/string/display.hpp"
#include "y/type/complex.hpp"
#include "y/container/matrix.hpp"
#include "y/mpl/rational.hpp"

using namespace upsylon;

#define TS(TYPE) do { const type_spec &ts = type_spec::of<TYPE>(); \
string_display::align(std::cerr,#TYPE,20)    << " => ";\
std::cerr << ts << std::endl;\
} while(false)

struct Dummy
{
    struct Internal
    {
    };
};


Y_UTEST(spec)
{

    std::cerr << "sizeof(type_spec)=" << sizeof(type_spec) << std::endl;

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
    TS(Dummy::Internal);

    TS(complex<float>);
    TS(complex<double>);

    TS(matrix<float>);
    TS(matrix<mpz>);

    type_spec::display(std::cerr);

    const float h = 0.5;
    std::cerr << "type_name_of<float> = " << type_name_of<float>() << std::endl;
    std::cerr << "type_name_of(" << h << ")=" << type_name_of(h) << std::endl;

#define NAMED(TYPE,ID) do {\
std::cerr << "type_name_of<" << #TYPE << "> = " << type_name_of<TYPE>() << std::endl;\
if(#ID==type_spec_of<TYPE>()) std::cerr << "\tis  " << #ID << std::endl; else std::cerr << "\tNOT " << #ID << std::endl;\
} while(false)

    NAMED(float,f);
    NAMED(float,float);
    NAMED(float,double);

	std::cerr << "type_name_of<short>()=" << type_name_of<short>() << std::endl;

    std::cerr << "sizeof(type_spec)=" << sizeof(type_spec) << std::endl;
    
}
Y_UTEST_DONE()

#include "y/hashing/type-mark.hpp"
#include "y/associative/map.hpp"

Y_UTEST(spec_db)
{
    std::cerr << "query among: "<< std::endl;
    type_spec::display(std::cerr);

    map<type_mark,string,hashing::type_mark_hasher<> > db;

    for(int i=1;i<argc;++i)
    {
        const char      *id = argv[i];
        const type_spec &ts = type_spec::query( argv[i] );
        std::cerr << id << " => " << ts << std::endl;

        const type_mark key = ts;
        const string    value = id;
        (void) db.insert(key,value);
    }

    std::cerr << "db: " << db << std::endl;
}
Y_UTEST_DONE()
