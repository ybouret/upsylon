
#include "y/ink/image.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>
#include "y/ptr/arc.hpp"
#include "y/hashing/sha1.hpp"

using namespace upsylon;
using namespace ink;

namespace
{
    template <typename T>
    static inline void do_ops(const pixmap<T> &pxm, engine &par, engine &seq)
    {
        const size_t w = pxm.w;
        const size_t h = pxm.h;
        pixmap<T>    work(w,h);
        hashing::sha1 H;
        const uint64_t h0 = pxm.template __hash<uint64_t>(H);
        std::cerr << "h0=" << h0 << std::endl;
        work.copy(pxm);
        const uint64_t h1 = work.template __hash<uint64_t>(H);
        std::cerr << "h1=" << h1 << std::endl;
        work.copy(pxm,seq);
        const uint64_t hs = work.template __hash<uint64_t>(H);
        std::cerr << "hs=" << hs << std::endl;
        work.copy(pxm,par);
        const uint64_t hp = work.template __hash<uint64_t>(H);
        std::cerr << "hp=" << hp << std::endl;
        Y_ASSERT(h1==h0);
        Y_ASSERT(hs==h0);
        Y_ASSERT(hp==h0);

    }
}

Y_UTEST(pixmap)
{
    imageIO &img = image::instance().initialize();
    dispatcher __par  = new concurrent::simd();
    dispatcher __seq  = new concurrent::sequential_for();

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string    fn   = argv[iarg];
        pixmapf         pxmf = img.loadf(fn,0);
        pixmap1         pxm1 = img.load1(fn,0);
        pixmap3         pxm3 = img.load3(fn,0);
        pixmap4         pxm4 = img.load4(fn,0);
        const area      full = pxmf;
        engine          par(__par,full);
        engine          seq(__seq,full);

        do_ops(pxmf,par,seq);
        do_ops(pxm1,par,seq);
        do_ops(pxm3,par,seq);
        do_ops(pxm4,par,seq);

    }

}
Y_UTEST_DONE()

