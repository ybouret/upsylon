#include "y/ios/conveyor/primary.hpp"
#include "y/ios/conveyor/network.hpp"
#include "y/ios/conveyor/derived.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ovstream.hpp"
#include "support.hpp"
#include "y/type/mark.hpp"

namespace {
 
    template <typename T>
    static inline
    size_t append(ios::ostream        &fp,
                  ios::conveyor       &cvr,
                  sequence<T>         &data,
                  sequence<type_mark> &tmark,
                  sequence<size_t>    &count)
    {
        size_t n = 1+alea.leq(10);
        
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
    
    vector<type_mark> tmark;
    vector<size_t>    count;
    size_t            total = 0;
    
    for(size_t iter=1+alea.leq(10);iter>0;--iter)
    {
        
        total += append(target,ic,iv,tmark,count);
        total += append(target,dc,dv,tmark,count);
        total += append(target,sc,sv,tmark,count);
    }
    
    for(size_t i=1;i<=tmark.size();++i)
    {
        std::cerr << tmark[i]->name() << "\t*" << count[i] << std::endl;
    }
    std::cerr << std::endl << "total= " << total << std::endl;
    Y_CHECK(target.size()==total);
    
    for(size_t i=1;i<=tmark.size();++i)
    {
        const size_t n = count[i];
        if( "int" == *tmark[i] )
        {
            std::cerr << "#int    = " << n << std::endl;
            continue;
        }
        
        if( "double" == *tmark[i] )
        {
            std::cerr << "#double = " << n << std::endl;
            continue;
        }
        
        if( "string" == *tmark[i] )
        {
            std::cerr << "#string = " << n << std::endl;
            continue;
        }
        
    
    }
    
    
}
Y_UTEST_DONE()

