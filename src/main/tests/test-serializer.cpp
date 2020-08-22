#include "y/ios/serializer.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "support.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"

#include "y/ptr/shared.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;

namespace {


    template <typename T>
    static inline size_t do_srz( ios::ostream &fp )
    {
        std::cerr << "Serializing " << type_name_of<T>() << std::endl;
        vector<T>             v;
        list<T>               l;
        list< shared_ptr<T> > m;

        for(size_t i=alea.leq(100);i>0;--i)
        {
            const T tmp = support::get<T>();
            v.push_back(tmp);
            l.push_back(tmp);
            {
                const shared_ptr<T> p = new T(tmp);
                m.push_back(p);
            }
        }


        size_t count = ios::serializer::save(fp,v);
        count       += ios::serializer::save(fp,l);
        count       += ios::serializer::save_meta(fp,m);
        
        return count;

    }

    
    template <typename T, typename LOADER>
    static inline
    void do_ld( ios::istream &fp, LOADER &loader)
    {
        std::cerr << "reloading " << type_name_of<T>() << std::endl;
        list<T> l;
        size_t shift = 0;
        for(size_t iter=0;iter<3;++iter)
        {
            ios::serializer::load(l, fp, loader, shift, *type_name_of<T>());
        }
        
    }
}


Y_UTEST(serializer)
{

    size_t total = 0;
    {
        ios::ocstream fp("srz.dat");
        total += do_srz<string>(fp);
        total += do_srz<apn>(fp);
        total += do_srz<apq>(fp);
    }
    std::cerr << "#written=" << total << std::endl;
    {
        ios::icstream fp("srz.dat");
        do_ld<string>(fp, string::read );
        do_ld<apn>(fp, apn::read );
        do_ld<apq>(fp, apq::read );

    }
}
Y_UTEST_DONE()

