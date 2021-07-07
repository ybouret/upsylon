#include "y/gfx/mask.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/heap.hpp"

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

    static inline void save_data(const accessible<size_t> &px,
                                 const accessible<size_t> &nl,
                                 const char *filename)
    {
        assert(px.size()==nl.size());
        ios::ocstream fp(filename);
        for(size_t i=1;i<=px.size();++i)
        {
            fp("%u %u\n", unsigned( px[i] ), unsigned( nl[i] ) );
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

    vector<size_t> w_px; // width first pixels
    vector<size_t> w_nl; // and number of leaves

    vector<size_t> h_px;
    vector<size_t> h_nl;



    for(unit_t w=1;w<=32;++w)
    {
        for(unit_t h=1;h<=32;++h)
        {
            const size_t px = h*w;
            mask m;
            fill_mask(m,w,h);
            std::cerr << w << "x" << h << ":";
            m.free();
            //m.stats(std::cerr);
            Y_ASSERT(px==m.capacity());
            const size_t leaves = m.get_tree().pool.size;
            std::cerr << m.get_tree().pool.size;
            std::cerr << std::endl;
            if(w>h)
            {
                w_px << px;
                w_nl << leaves;
            }
            else
            {
                if(w<h)
                {
                    h_px << px;
                    h_nl << leaves;
                }
                else
                {
                    w_px << px;
                    w_nl << leaves;
                    h_px << px;
                    h_nl << leaves;
                }
            }
        }
    }

    hsort(w_px,w_nl, comparison::increasing<size_t> );
    hsort(h_px,h_nl, comparison::increasing<size_t> );

    save_data(w_px,w_nl,"mask-w.dat");
    save_data(h_px,h_nl,"mask-h.dat");





}
Y_UTEST_DONE()

