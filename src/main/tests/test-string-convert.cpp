#include "y/string/convert.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(string_convert)
{
    for(int i=1;i<argc;)
    {
        const string type = argv[i++];
        if(i>=argc) throw exception("missing value string after '%s'",*type);
        const string data = argv[i++];

        if(type.size()!=1) throw exception("type must be 1 character long");
        switch(type[0])
        {
            case 's': std::cerr << "to<size_t>(" << data << ")=";
            {
                const size_t s = string_convert::to<size_t>(data);
                std::cerr << s << std::endl;
            }
                break;

            case 'i': std::cerr << "to<unit_t>(" << data << ")=";
            {
                const unit_t j = string_convert::to<unit_t>(data);
                std::cerr << j << std::endl;
            }
                break;

            case 'f': std::cerr << "to<float>(" << data << ")=";
            {
                const float f = string_convert::to<float>(data);
                std::cerr << f << std::endl;
            }
                break;

            case 'd': std::cerr << "to<double>(" << data << ")=";
            {
                const double d = string_convert::to<double>(data);
                std::cerr << d << std::endl;
            }
                break;

            default:
                throw exception("unknown type '%s'", *type);
        }

    }
}
Y_UTEST_DONE()

