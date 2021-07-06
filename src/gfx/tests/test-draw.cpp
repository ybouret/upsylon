#include "y/gfx/draw/line.hpp"
#include "y/gfx/draw/circle.hpp"
#include "y/gfx/draw/fill.hpp"
#include "y/gfx/draw/mask.hpp"

#include "y/utest/run.hpp"

#include "y/gfx/image/io.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/gfx/color/named.hpp"
#include "y/gfx/color/convert.hpp"

using namespace upsylon;
using namespace graphic;

const unit_t w = 640;
const unit_t h = 480;

const unit_t xmin = -w/2;
const unit_t xmax = w+w/2;
const unit_t ymin = -h/2;
const unit_t ymax = h+h/2;

static inline unit_t  xr() { return alea.range<unit_t>(xmin,xmax); }
static inline unit_t  yr() { return alea.range<unit_t>(ymin,ymax); }
static inline uint8_t ar() { return alea.full<uint8_t>();  }

static inline bool is_too_far(const accessible<uint8_t> &, const coord &p) throw()
{
    const coord  delta = coord(w/2,h/2)-p;
    const unit_t d2    = delta.norm2();
    return d2>200*200;
}


Y_UTEST(draw)
{
    Y_USE_IMG();

    std::cerr << "building..." << std::endl;
    mask            m;
    pixmap<float>   pxmf(w,h);
    pixmap<uint8_t> pxm1(w,h);
    pixmap<rgb>     pxm3(w,h);
    pixmap<rgba>    pxm4(w,h);
    pixmap<rgb>     pxmm(w,h);

    std::cerr << "hline" << std::endl;

    for(size_t i=2+alea.leq(30);i>0;--i)
    {
        const unit_t  x0 = xr();
        const unit_t  y0 = yr();
        const unit_t  x1 = xr();
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        draw::hline(pxmf,x0,y0,x1,f);
        draw::hline(pxm1,x0,y0,x1,b);
        draw::hline(pxm3,x0,y0,x1,c);
        draw::hline(pxm4,x0,y0,x1,c);
        draw::hline(pxmf,x0,y0,x1,m);
    }

    for(size_t i=2+alea.leq(30);i>0;--i)
    {
        const unit_t  x0 = xr();
        const unit_t  y0 = yr();
        const unit_t  x1 = xr();
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        const uint8_t a  = ar();
        draw::hline(pxmf,x0,y0,x1,f,a);
        draw::hline(pxm1,x0,y0,x1,b,a);
        draw::hline(pxm3,x0,y0,x1,c,a);
        draw::hline(pxm4,x0,y0,x1,c,a);
        draw::hline(pxmf,x0,y0,x1,m);
    }

    std::cerr << "vline" << std::endl;

    for(size_t i=2+alea.leq(30);i>0;--i)
    {
        const unit_t  x0 = xr();
        const unit_t  y0 = yr();
        const unit_t  y1 = yr();
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        draw::vline(pxmf,x0,y0,y1,f);
        draw::vline(pxm1,x0,y0,y1,b);
        draw::vline(pxm3,x0,y0,y1,c);
        draw::vline(pxm4,x0,y0,y1,c);
        draw::vline(pxmf,x0,y0,y1,m);
    }



    for(size_t i=2+alea.leq(30);i>0;--i)
    {
        const unit_t  x0 = xr();
        const unit_t  y0 = yr();
        const unit_t  y1 = yr();
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        const uint8_t a  = ar();
        draw::vline(pxmf,x0,y0,y1,f,a);
        draw::vline(pxm1,x0,y0,y1,b,a);
        draw::vline(pxm3,x0,y0,y1,c,a);
        draw::vline(pxm4,x0,y0,y1,c,a);
        draw::vline(pxmf,x0,y0,y1,m);
    }

    std::cerr << "line" << std::endl;

    for(size_t i=2+alea.leq(30);i>0;--i)
    {
        const unit_t  x0 = xr();
        const unit_t  y0 = yr();
        const unit_t  x1 = xr();
        const unit_t  y1 = yr();
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        draw::line(pxmf,x0,y0,x1,y1,f);
        draw::line(pxm1,x0,y0,x1,y1,b);
        draw::line(pxm3,x0,y0,x1,y1,c);
        draw::line(pxm4,x0,y0,x1,y1,c);
        draw::line(pxmf,x0,y0,x1,y1,m);
    }

    for(size_t i=2+alea.leq(30);i>0;--i)
    {
        const unit_t  x0 = xr();
        const unit_t  y0 = yr();
        const unit_t  x1 = xr();
        const unit_t  y1 = yr();
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        const uint8_t a  = ar();
        draw::line(pxmf,x0,y0,x1,y1,f,a);
        draw::line(pxm1,x0,y0,x1,y1,b,a);
        draw::line(pxm3,x0,y0,x1,y1,c,a);
        draw::line(pxm4,x0,y0,x1,y1,c,a);
        draw::line(pxmf,x0,y0,x1,y1,m);
    }


    std::cerr << "circle" << std::endl;

    for(size_t i=2+alea.leq(30);i>0;--i)
    {
        const unit_t  x0 = xr();
        const unit_t  y0 = yr();
        const unit_t  r  = alea.range<unit_t>(0,256);
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        draw::circle(pxmf,x0,y0,r,f);
        draw::circle(pxm1,x0,y0,r,b);
        draw::circle(pxm3,x0,y0,r,c);
        draw::circle(pxm4,x0,y0,r,c);
        draw::circle(pxmf,x0,y0,r,m);
    }


    for(size_t i=2+alea.leq(30);i>0;--i)
    {
        const unit_t  x0 = xr();
        const unit_t  y0 = yr();
        const unit_t  r  = alea.range<unit_t>(0,256);
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        const uint8_t a  = ar();
        draw::circle(pxmf,x0,y0,r,f,a);
        draw::circle(pxm1,x0,y0,r,b,a);
        draw::circle(pxm3,x0,y0,r,c,a);
        draw::circle(pxm4,x0,y0,r,c,a);
        draw::circle(pxmf,x0,y0,r,m);
    }

    std::cerr << "disk" << std::endl;

    for(size_t i=2+alea.leq(8);i>0;--i)
    {
        const unit_t  x0 = xr();
        const unit_t  y0 = yr();
        const unit_t  r  = alea.range<unit_t>(0,100);
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        draw::disk(pxmf,x0,y0,r,f);
        draw::disk(pxm1,x0,y0,r,b);
        draw::disk(pxm3,x0,y0,r,c);
        draw::disk(pxm4,x0,y0,r,c);
        draw::disk(pxmf,x0,y0,r,m);
    }


    for(size_t i=2+alea.leq(8);i>0;--i)
    {
        const unit_t  x0 = xr();
        const unit_t  y0 = yr();
        const unit_t  r  = alea.range<unit_t>(0,100);
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        const uint8_t a  = ar();
        draw::disk(pxmf,x0,y0,r,f,a);
        draw::disk(pxm1,x0,y0,r,b,a);
        draw::disk(pxm3,x0,y0,r,c,a);
        draw::disk(pxm4,x0,y0,r,c,a);
        draw::disk(pxmf,x0,y0,r,m);
    }

    std::cerr << "fill" << std::endl;

    for(size_t i=2+alea.leq(4);i>0;--i)
    {
        const unit_t  x0 = alea.range<unit_t>(0,w);
        const unit_t  y0 = alea.range<unit_t>(0,h);
        const unit_t  x1 = x0 + alea.range<unit_t>(-100,100);
        const unit_t  y1 = y0 + alea.range<unit_t>(-100,100);
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        draw::fill(pxmf,x0,y0,x1,y1,f);
        draw::fill(pxm1,x0,y0,x1,y1,b);
        draw::fill(pxm3,x0,y0,x1,y1,c);
        draw::fill(pxm4,x0,y0,x1,y1,c);
        draw::fill(pxmf,x0,y0,x1,y1,m);
    }


    for(size_t i=2+alea.leq(4);i>0;--i)
    {
        const unit_t  x0 = alea.range<unit_t>(0,w);
        const unit_t  y0 = alea.range<unit_t>(0,h);
        const unit_t  x1 = x0 + alea.range<unit_t>(-100,100);
        const unit_t  y1 = y0 + alea.range<unit_t>(-100,100);
        const rgba    c  = named_color::rand(alea);
        const uint8_t b  = convert<uint8_t,rgba>::from(c);
        const float   f  = convert<float,uint8_t>::from(b);
        const uint8_t a  = ar();
        draw::fill(pxmf,x0,y0,x1,y1,f,a);
        draw::fill(pxm1,x0,y0,x1,y1,b,a);
        draw::fill(pxm3,x0,y0,x1,y1,c,a);
        draw::fill(pxm4,x0,y0,x1,y1,c,a);
        draw::fill(pxmf,x0,y0,x1,y1,m);
    }



    std::cerr << "#mask=" << m.size() << std::endl;

    std::cerr << "masking" << std::endl;
    m.remove_if(is_too_far);

    draw::masking(pxmm,m,Y_BLUE);

    {
        mask::raw_keys blist;
        m.collect_keys(blist);
        alea.shuffle(blist);
        for(size_t i=blist.size/2;i>0;--i)
        {
            delete blist.pop_back();
        }
        while(blist.size)
        {
            Y_ASSERT(m.remove_at( *(blist.tail) ));
            delete blist.pop_back();
        }
    }


    draw::masking(pxmm,m,Y_GREEN,100);


    std::cerr << "saving..." << std::endl;

    IMG.save(pxmf,"drawf.png");
    IMG.save(pxm1,"draw1.png");
    IMG.save(pxm3,"draw3.png");
    IMG.save(pxm4,"draw4.png");
    IMG.save(pxmm,"drawm.png");

    

}
Y_UTEST_DONE()

