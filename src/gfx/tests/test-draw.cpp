#include "y/gfx/draw/line.hpp"
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


Y_UTEST(draw)
{
    Y_USE_IMG();

    std::cerr << "building..." << std::endl;
    mask            m;
    pixmap<float>   pxmf(w,h);
    pixmap<uint8_t> pxm1(w,h);
    pixmap<rgb>     pxm3(w,h);
    pixmap<rgba>    pxm4(w,h);
    pixmap<uint8_t> pxmm(w,h);

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


    for(mask::iterator it=m.begin();it!=m.end();++it)
    {
        pxmm[*it] = 0xff;
    }

    std::cerr << "saving..." << std::endl;

    IMG.save(pxmf,"drawf.png");
    IMG.save(pxm1,"draw1.png");
    IMG.save(pxm3,"draw3.png");
    IMG.save(pxm4,"draw4.png");
    IMG.save(pxmm,"drawm.png");

    

}
Y_UTEST_DONE()

