#include "y/program.hpp"
#include "y/mpl/natural.hpp"
#include "y/ios/orstream.hpp"
#include "y/ios/osstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/string/convert.hpp"
#include "y/string/io.hpp"
#include "y/hashing/sha1.hpp"
#include <cmath>

using namespace upsylon;

Y_PROGRAM_START()
{
    size_t        po2 = 8;
    
    if(argc>1)
    {
        po2 = string_convert::to<size_t>(argv[1],"po2");
    }
    const size_t  max_size = 1<<po2;
    size_t        written  = 0;
    hashing::sha1 hasher;
    {
        ios::orstream fp("mprimes.bin");
        uint32_t      count    = 0;
        fp.emit<uint32_t>(count);
        written += sizeof(count);

        mpn last_p = 0;
        hasher.set();
        for(mpn p=2;;p=mpn::next_prime(++p))
        {
            const size_t to_write = p.save_length();
            if(written+to_write>max_size)
            {
                break;
            }
            //std::cerr << p << '.';
            std::cerr << '.';
            written += p.save(fp);
            last_p = p;
            hasher(p);
            ++count;
        }
        std::cerr << "->" << last_p << std::endl;
        fp.rewind();
        fp.emit<uint32_t>(count);
        std::cerr << "#written= " << written << std::endl;
        std::cerr << "#count  = " << count   << std::endl;
        std::cerr << "empty   = " << int(floor( (1e6 * (double(max_size)-double(written)) ) / double(max_size) + 0.5 )) << "ppm" << std::endl;
        std::cerr << "bpp     = " << double(written)/count << std::endl;
    }
    const digest mdw = hasher.md();
    
    hasher.set();
    {
        ios::icstream  fp("mprimes.bin");
        const uint32_t count = fp.read<uint32_t>();
        size_t         nread = sizeof(count);
        for(uint32_t i=0;i<count;++i)
        {
            size_t    nr = 0;
            const mpn p = mpn::read(fp,&nr);
            std::cerr << '.';
            nread += nr;
            hasher(p);
        }
        std::cerr << std::endl;
        std::cerr << "input.nread=" << nread << std::endl;
        std::cerr << "input.count=" << count << std::endl;
    }
    const digest mdr = hasher.md();
    std::cerr << "mdw=" << mdw << std::endl;
    std::cerr << "mdr=" << mdr << std::endl;
    if(mdw!=mdr) throw exception("corrupted data");

}
Y_PROGRAM_END()

