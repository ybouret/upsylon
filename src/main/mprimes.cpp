#include "y/program.hpp"
#include "y/mpl/natural.hpp"
#include "y/ios/orstream.hpp"
#include "y/ios/osstream.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

Y_PROGRAM_START()
{
    size_t        max_size = 100;
    if(argc>1)
    {
        max_size = string_convert::to<size_t>(argv[1],"max_size");
    }

    ios::orstream fp("mprimes.bin");
    uint32_t      count    = 0;
    fp.emit<uint32_t>(count);
    size_t written = sizeof(count);

    for(mpn p=2;;p=mpn::next_prime(++p))
    {
        std::cerr << p << '.';
        const size_t to_write = p.save_length();
        if(written+to_write>max_size)
        {
            break;
        }
        written += p.save(fp);
        ++count;
    }
    std::cerr << std::endl;
    fp.rewind();
    fp.emit<uint32_t>(count);
    std::cerr << "#written=" << written << std::endl;
    std::cerr << "#count  =" << count   << std::endl;
}
Y_PROGRAM_END()

