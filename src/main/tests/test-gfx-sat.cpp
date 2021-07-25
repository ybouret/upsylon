#include "y/utest/run.hpp"
#include "y/gfx/color/rgb.hpp"

#include "y/ios/ocstream.hpp"
#include "y/type/utils.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    typedef vector<rgb> rgbs;
}

Y_UTEST(gfx_sat)
{
    
    rgbs db[256];
    int  ntot = 0;
    for(int i=255;i>=0;--i)
    {
        for(int j=i;j>=0;--j)
        {
            for(int k=j;k>=0;--k)
            {
                const rgb sat(i,j,k);
                
                
                ++ntot;
            }
        }
    }
    
    std::cerr << "ntot=" << ntot << std::endl;
    
    
    
#if 0
    size_t          off[256] = { 0 };
    size_t          num[256] = { 0 };
    vector<uint8_t> data(2048,as_capacity);

    memset(off,0,sizeof(off));
    memset(num,0,sizeof(num));

    size_t ndiv = 0;
    size_t nmax = 0;
    for(size_t i=1;i<256;++i)
    {
        std::cerr << std::setw(3) << i << " :";
        size_t ntmp = 0;
        for(size_t j=2;j<=i;++j)
        {
            if(0==(i%j))
            {
                std::cerr << ' ' << std::setw(3) << j;
                ++ndiv;
                ++ntmp;
                data.push_back( uint8_t(j) );
            }
        }
        nmax = max_of(ntmp,nmax);
        off[i] = off[i-1] + num[i-1];
        num[i] = ntmp;
        std::cerr << std::endl;
    }
    Y_CHECK(0==off[0]);
    
    std::cerr << "ndiv=" << ndiv << std::endl;
    std::cerr << "nmax=" << nmax << std::endl;

    ios::ocstream fp("bred-inc.hxx");
    fp("static const uint8_t bred[%u] = {\n", unsigned(ndiv));
    fp(" 0x%02x", unsigned(data[1]));
    for(size_t i=2;i<=ndiv;++i)
    {
        fp(",");
        if(0==((i-1)%16)) fp << '\n';
        fp(" 0x%02x", unsigned(data[i]));
    }
    fp("\n};\n\n");

    fp << "const byte_divider byte_divider:: table[256] = {\n";
    for(size_t i=0;i<256;++i)
    {
        fp("\tbyte_divider(&bred[%u],%u)", unsigned(off[i]), unsigned(num[i]));
        if(i<255) fp << ',';
        fp("\n");
    }
    fp << "};\n";

    for(size_t i=0;i<256;++i)
    {
        const byte_divider &b = byte_divider::table[i];
        std::cerr << i << std::endl;
        for(size_t j=0;j<b.count;++j)
        {
            const unsigned d = b.value[j];
            std::cerr << " " << d;
            if( 0 !=(i%d) ) throw exception("bad divider");
        }
        std::cerr << std::endl;
    }
#endif
    


#if 0
    std::cerr << "Build database of saturated samples" << std::endl;
    rgb_db db(num_sat,as_capacity);
    for(int j=255;j>=0;--j)
    {
        for(int k=j;k>=0;--k)
        {
            const rgb c(0xff, uint8_t(j), uint8_t(k));
            const entry::ptr p = new entry(c);
            Y_ASSERT(db.insert(p));
        }
    }
    Y_CHECK(num_sat==db.size());

    std::cerr << "Checking all" << std::endl;
    std::cerr << "[";
    for(int i=255;i>=0;--i)
    {
        if(0==(i%8)) (std::cerr << ".").flush();
        const bool zi = (0==i);
        for(int j=i;j>=0;--j)
        {
            const bool zij = zi && (0==j);
            for(int k=j;k>=0;--k)
            {
                if(zij&& (0==k)) continue;;
                const rgb src(static_cast<uint8_t>(i),
                              static_cast<uint8_t>(j),
                              static_cast<uint8_t>(k) );
                const rgb   sat = src.saturated();
                entry::ptr *ppe = db.search(sat);
                Y_ASSERT(ppe);
                Y_ASSERT((**ppe).data.insert(src));
            }
        }
    }
    std::cerr << "]\n";
    size_t idx = 0;
    for(rgb_db::iterator it=db.begin();it!=db.end();++it)
    {
        ++idx;
        const entry &e = **it;
        std::cerr << idx << " : " << e.c << " => ";
        std::cerr << "#" << e.data.size();
        std::cerr << std::endl;
    }
#endif

}
Y_UTEST_DONE()
 
