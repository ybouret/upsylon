#include "y/utest/run.hpp"
#include "y/gfx/color/rgb.hpp"
#include "y/associative/hash/set.hpp"
#include "y/ordered/sorted-vector.hpp"
#include "y/ptr/intr.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    static const size_t                num_sat = (256*257)>>1;
    typedef sorted_vector<rgb>         sorted_rgb;
    typedef ordered_unique<sorted_rgb> unique_rgb;

    class rgb_hasher
    {
    public:
        inline  rgb_hasher() throw() {}
        inline ~rgb_hasher() throw() {}

        size_t operator()(const rgb &c) const throw()
        {
            union
            {
                size_t  key;
                uint8_t arr[sizeof(size_t)];
            } alias = { 0 };
            alias.arr[0] = c.r;
            alias.arr[1] = c.g;
            alias.arr[2] = c.b;
            return alias.key;
        }
    };

    class entry : public object, public counted
    {
    public:
        typedef intr_ptr<rgb,entry> ptr;
        const rgb  c;
        unique_rgb data;

        inline explicit entry(const rgb &_) throw() : object(), c(_), data()
        {
        }

        inline virtual ~entry() throw() {}

        const rgb & key() const throw()
        {
            return c;
        }

    };

    typedef hash_set<rgb,entry::ptr,rgb_hasher> rgb_db;


}


Y_UTEST(gfx_sat)
{

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

}
Y_UTEST_DONE()
 
