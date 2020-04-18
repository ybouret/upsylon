#include "y/ios/conveyor/primary.hpp"
#include "y/ios/conveyor/network.hpp"
#include "y/ios/conveyor/derived.hpp"
#include "y/ios/conveyor/tuple.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ovstream.hpp"
#include "support.hpp"
#include "y/type/mark.hpp"
#include "y/ios/imstream.hpp"

namespace {
    
    template <typename T>
    static inline
    size_t append(ios::ostream        &fp,
                  ios::conveyor       &cvr,
                  sequence<T>         &data,
                  sequence<type_mark> &tmark,
                  sequence<size_t>    &count)
    {
        size_t n = 1+alea.leq(100);
        
        count.push_back(n);
        const type_mark tm = typeid(T);
        tmark.push_back(tm);
        
        size_t total = 0;
        while(n-- > 0)
        {
            const T temp = support::get<T>();
            data.push_back(temp);
            total += cvr.save(fp, &temp);
        }
        return total;
    }
    
}

using namespace upsylon;

Y_UTEST(conveyor)
{
    ios::ovstream target;
    
    ios::primary_conveyor<int>    ic; vector<int>    iv;
    ios::network_conveyor<double> dc; vector<double> dv;
    ios::derived_conveyor<string> sc; vector<string> sv;
    ios::derived_conveyor<mpq>    qc; vector<mpq>    qv;
    ios::tuple_conveyor<complex,float,ios::primary_conveyor>  cc; vector< complex<float> >  cv;
    ios::tuple_conveyor<point3d,double,ios::network_conveyor> pc; vector< point3d<double> > pv;

    
    
    vector<type_mark> tmark;
    vector<size_t>    count;
    size_t            total = 0;
    
    for(size_t iter=1+alea.leq(100);iter>0;--iter)
    {
        
        total += append(target,ic,iv,tmark,count);
        total += append(target,dc,dv,tmark,count);
        total += append(target,sc,sv,tmark,count);
        total += append(target,qc,qv,tmark,count);
        total += append(target,cc,cv,tmark,count);
        total += append(target,cc,cv,tmark,count);
        total += append(target,pc,pv,tmark,count);

    }
    
    std::cerr << "total= " << total << std::endl;
    Y_CHECK(target.size()==total);
    
    std::cerr << std::endl << "reloading..." << std::endl;
    
    ios::imstream source(target);
    
    size_t ji = 0;
    size_t jd = 0;
    size_t js = 0;
    size_t jq = 0;
    size_t jc = 0;
    size_t jp = 0;
    
    for(size_t i=1;i<=tmark.size();++i)
    {
        const size_t n = count[i];
        if( "int" == *tmark[i] )
        {
            int r = 0;
            for(size_t i=1;i<=n;++i)
            {
                if(++ji>iv.size()) throw exception("too many ints");
                ic.load(&r,source);
                Y_ASSERT( r == iv[ji] );
            }
            continue;
        }
        
        if( "double" == *tmark[i] )
        {
            double d = 0;
            for(size_t i=1;i<=n;++i)
            {
                if(++jd>dv.size()) throw exception("too many doubles");
                dc.load(&d,source);
                Y_ASSERT( 0 == memcmp(&d, &dv[jd], sizeof(double)));
            }
            continue;
        }
        
        if( "string" == *tmark[i] )
        {
            string s;
            for(size_t i=1;i<=n;++i)
            {
                if(++js>sv.size()) throw exception("too many strings");
                sc.load(&s,source);
                Y_ASSERT(s==sv[js]);
            }
            continue;
        }
        
        if( "mpq" == *tmark[i] )
        {
            mpq q;
            for(size_t i=1;i<=n;++i)
            {
                if(++jq>qv.size()) throw exception("too many Qs");
                qc.load(&q,source);
                Y_ASSERT(q==qv[jq]);
            }
            continue;
        }
        
        if( "complex<float>" == *tmark[i] )
        {
            complex<float> c;
            for(size_t i=1;i<=n;++i)
            {
                if(++jc>cv.size()) throw exception("too many complexes");
                cc.load(&c,source);
                Y_ASSERT( 0 == memcmp(&c, &cv[jc], sizeof(complex<float>)));
            }
            continue;
        }
        
        
        if( "point3d<double>" == *tmark[i] )
        {
            point3d<double> p;
            
            for(size_t i=1;i<=n;++i)
            {
                if(++jp>pv.size()) throw exception("too many p3d");
                pc.load(&p,source);
                Y_ASSERT( 0 == memcmp(&p, &pv[jp], sizeof(point3d<double>)));
            }
            continue;
        }
        
        throw exception("unhandled <%s>", *(tmark[i]->name()) );
    }
    
    
}
Y_UTEST_DONE()

