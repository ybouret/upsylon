#include "y/counting/part.hpp"
#include "y/utest/run.hpp"
#include "y/counting/perm.hpp"
#include "y/yap/natural.hpp"

#include <iomanip>


using namespace upsylon;


#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"
#include "y/hashing/sha1.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"

Y_UTEST(part)
{
    size_t nmax = 5;
    if( argc > 1 )
    {
        nmax = string_convert::to<size_t>(argv[1],"n");
    }

    typedef uint32_t                      key_t;
    typedef vector<size_t,memory::pooled> vec_t;

    vector<key_t> keys;
    vector<vec_t> vecs;

    for(size_t n=1;n<=nmax;++n)
    {
        integer_partition  pb(n);
        const size_t       ns = pb.outcomes();
        std::cerr << "<" << n << "> can be summed in " << ns << " outcomes" << std::endl;

        pb.initialize();
        size_t count = 0;
        apn    mpBell = 0;
        size_t szBell = 0;
        do
        {
            keys.free();
            vecs.free();
            ++count;
            std::cerr << pb << " ->";
            const apn nc = pb.configs( counting::with_ap );
            std::cerr << " +" << nc << std::endl;
            mpBell += nc;
            const size_t sz = pb.configs(counting::with_sz);
            szBell += sz;



            permutation          perm( pb.size() );
            hashing::sha1        H;
            vec_t p( perm.size(), 0 );
            for( perm.boot(); perm.good(); perm.next() )
            {
                perm.make(p,pb);
                const key_t k = H.key<key_t>(p);
                bool found   = false;
                //bool collide = false;
                for(size_t i=keys.size();i>0;--i)
                {
                    if(k==keys[i])
                    {
                        found = true;
                        vec_t &v=vecs[i];
                        assert(v.size()==p.size());
                        if( 0 != memcmp( &v[1], &p[1], sizeof(size_t)*v.size()) )
                        {
                            std::cerr << "collision of " << p << "/" << v << std::endl;
                            //collide = true;
                        }
                        break;
                    }
                }
                if( !found )
                {
                    //std::cerr << "\t" << p << " / " << k << std::endl;
                    keys << k;
                    vecs << p;
                    if( keys.size() >= sz )
                    {
                        break;
                    }
                }
            }


        }
        while( pb.build_next() );
        Y_CHECK(ns==count);
        std::cerr << "#outcomes = " << ns     << std::endl;
        std::cerr << "#mpBell   = " << mpBell << std::endl;
        std::cerr << "#szBell   = " << szBell << std::endl;
    }

    std::cerr << "Checking copy : [";
    for(size_t n=1;n<=nmax;++n)
    {
        integer_partition  part(n);
        do {
            integer_partition temp(part);
            Y_ASSERT(temp.size()==part.size());
            for(size_t j=temp.size();j>0;--j)
            {
                Y_ASSERT(temp[j]==part[j]);
            }
            while( temp.build_next() )
                ;
        } while(part.build_next());
        std::cerr << '.';
    }
    std::cerr << "]" << std::endl;




}
Y_UTEST_DONE()

#include "y/counting/parts.hpp"

Y_UTEST(parts)
{
    size_t nmax = 5;
    if( argc > 1 )
    {
        nmax = string_convert::to<size_t>(argv[1],"n");
    }

    for(size_t n=1;n<=nmax;++n)
    {
        integer_partitions parts(n);
        std::cerr << "[" << n << "] #" << parts.count << std::endl;

        bool ret = false;
        for( parts.boot(); parts.good(); parts.next() )
        {

            std::cerr << parts;
            if( (parts.index%4) == 0 )
            {
                ret = true;
                std::cerr << std::endl;
            }
            else
            {
                ret=false;
                std::cerr << " ";
            }
            integer_partitions temp(parts);
            Y_ASSERT(temp.index==parts.index);
            Y_ASSERT(temp.count==parts.count);
        }
        if(!ret)
        std::cerr << std::endl;
    }


}
Y_UTEST_DONE()



Y_UTEST(bell)
{
    const char  *BellTab[] =
    {
        "1",
        "1",
        "2",
        "5",
        "15",
        "52",
        "203",
        "877",
        "4140",
        "21147",
        "115975",
        "678570",
        "4213597",
        "27644437",
        "190899322",
        "1382958545",
        "10480142147",
        "82864869804",
        "682076806159",
        "5832742205057",
        "51724158235372",
        "474869816156751",
        "4506715738447323",
        "44152005855084346",
        "445958869294805289",
        "4638590332229999353",
        "49631246523618756274"
    };

    size_t i32=0;
    size_t i64=0;
    for(size_t i=1;i<sizeof(BellTab)/sizeof(BellTab[0]);++i)
    {
        const apn BellNumber = apn::parse(BellTab[i]);
        (std::cerr << std::setw(3) << i << ' ' << BellNumber << ' ').flush();
        const apn mpBell     = integer_partition::Bell<apn>(i);
        Y_ASSERT(BellNumber==mpBell);
        uint32_t v32 = 0;
        uint64_t v64 = 0;
        if( mpBell.to(v32) )
        {
            i32 = i;
            std::cerr  << '+';
        }
        else
        {
            std::cerr << '*';
        }
        if( mpBell.to(v64) )
        {
            i64 = i;
            std::cerr << '+';
        }
        else
        {
            std::cerr << '*';
        }
        std::cerr << std::endl;
    }
    std::cerr << "\ti32max=" << i32 << std::endl;
    std::cerr << "\ti64max=" << i64 << std::endl;
}
Y_UTEST_DONE()
