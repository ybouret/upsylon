#include "y/gfx/mask.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace graphic;


namespace
{
    static inline
    void fill_mask( mask &m, const unit_t w, const unit_t h)
    {
        m.free();
        coord p;
        size_t n=0;
        for(p.x=0;p.x<w;++p.x)
        {
            for(p.y=0;p.y<h;++p.y)
            {
                m.insert(p);
                Y_ASSERT(++n==m.size());
            }
        }
    }
}

Y_UTEST(mask)
{

    {
        mask m;


        m.insert( coord(0,0) );
        m.insert( coord(1,0) );
        m.insert( coord(0,1) );
        m.insert( coord(1,1) );
        m.get_root().graphViz("mask.dot");
        for(mask::iterator it=m.begin();it!=m.end();++it)
        {
            std::cerr << *it << std::endl;
        }

        m.free();

        for(unit_t i=-3;i<=3;++i)
        {
            for(unit_t j=-3;j<=3;++j)
            {
                m += coord(i,j);
            }
        }

        for(mask::iterator it=m.begin();it!=m.end();++it)
        {
            std::cerr << *it << std::endl;
        }
        m.get_root().graphViz("mask3x3.dot");
        m.stats(std::cerr);
        m.free();
        m.stats(std::cerr);

        std::cerr << std::endl;
    }

    std::cerr << "Filling..." << std::endl;
    for(unit_t w=1;w<=16;++w)
    {
        for(unit_t h=1;h<=16;++h)
        {
            mask m;
            fill_mask(m,w,h);
            std::cerr << w << "x" << h << ":";
            m.free();
            //m.stats(std::cerr);
            Y_ASSERT(size_t(w*h)==m.capacity());
            std::cerr << m.get_tree().pool.size;
            std::cerr << std::endl;
        }
    }



}
Y_UTEST_DONE()

