
#include "y/gfx/ops/blobs.hpp"
#include "y/gfx/ops/hist.hpp"
#include "y/gfx/ops/keep.hpp"


#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include "y/utest/sizeof.hpp"

#include "y/gfx/image/io.hpp"

#include "y/ios/ocstream.hpp"
#include "y/associative/hash/map.hpp"

using namespace upsylon;
using namespace graphic;


Y_UTEST(blobs)
{

    image::io &IMG       = Y_IMAGE();
    engine     seqEngine = new concurrent::solo();
    engine     parEngine = new concurrent::simt();


    {
        typedef hash_map<unit_t,size_t> db_type;
        db_type db4,db8;

        for(unit_t w=1;w<=128;++w)
        {
            for(unit_t h=1;h<=128;++h)
            {
                const area   a(w,h);
                const size_t n4 = blobs::max_in(a,blobs::connect4);
                const size_t n8 = blobs::max_in(a,blobs::connect8);
                {
                    size_t *p = db4.search(a.items);
                    if(p)  *p = max_of(*p,n4);
                    else
                    {
                        (void) db4.insert(a.items,n4);
                    }
                }

                {
                    size_t *p = db8.search(a.items);
                    if(p)  *p = max_of(*p,n8);
                    else
                    {
                        (void) db8.insert(a.items,n8);
                    }
                }
            }
        }
        db4.sort_keys_with(comparison::increasing<long>);
        {
            ios::ocstream fp("max4.dat");
            for(db_type::iterator it=db4.begin();it!=db4.end();++it)
            {
                const long x = long( it.get().ckey );
                const long n = long(*it);
                fp("%ld %ld %g\n",x,n,double(n)-double(x)/2.0);
            }
        }

        db8.sort_keys_with(comparison::increasing<long>);
        {
            ios::ocstream fp("max8.dat");
            for(db_type::iterator it=db8.begin();it!=db8.end();++it)
            {
                const long x = long( it.get().ckey );
                const long n = long(*it);
                fp("%ld %ld %g\n",x,n,double(n)-double(x)/4.0);
            }
        }
    }


    if(argc>1)
    {
        const pixmap<rgb> img = IMG.load<rgb>(argv[1]);
        IMG.save(img,"img.png");
        std::cerr << "loaded: " << img << std::endl;
        
        broker       seq(seqEngine,img);
        broker       par(parEngine,img);
        pixmap<rgb>  fg(img.w,img.h);

        {
            histogram H;
            compute_histogram::update(H,img,par, convert<uint8_t,rgb>::from );
            const uint8_t t = H.threshold();
            keep::transfer(fg,img,par,convert<uint8_t,rgb>::from,keep::geq,t,rgb(0,0,0));
            IMG.save(fg,"fg.png");
        }

        blobs b(img.w,img.h);
        
    }

}
Y_UTEST_DONE()

