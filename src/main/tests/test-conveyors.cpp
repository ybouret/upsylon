
#include "y/ios/conveyors.hpp"
#include "y/utest/sizeof.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/sequence/list.hpp"

#include "support.hpp"

using namespace upsylon;

namespace {
    
    template <typename T>
    void doTest( const ios::conveyor &cnv )
    {
        std::cerr << "testing [" << cnv << "]" << std::endl;
        vector<T>     source;
        vector<T>     target;
        list<T>       target2;
        ios::ovstream block;
        const size_t  items = alea.leq(1024);
        size_t        total = 0;
        for(size_t i=0;i<items;++i)
        {
            // fill original
            {
                const T tmp = support::get<T>();
                source.push_back(tmp);
            }
            
            //fill with something else
            {
                const T tmp = support::get<T>();
                target.push_back(tmp);
                target2.push_back(tmp);
            }
            
            // save original data into block
            total += cnv.save(block,&source.back());
        }
        std::cerr << "\t#items=" << items << " -> #bytes=" << block.size() << std::endl;
        std::cerr << "\t"; Y_CHECK(total==block.size());
        total = 0;
        for(size_t i=1;i<=items;++i)
        {
            total += cnv.copy(&target[i], &source[i]);
        }
        std::cerr << "\t"; Y_CHECK(total==sizeof(T)*source.size());
        
        {
        ios::imstream fp(block);
        total = 0;
        for(size_t i=1;i<=items;++i)
        {
            total +=  cnv.load(&target[i], fp);
            Y_ASSERT(target[i]==source[i]);
        }
        }
        
        std::cerr << "\t"; Y_CHECK(total==block.size());
        
        cnv.copy_block(target2,source);
        for(size_t i=1;i<=items;++i)
        {
            Y_ASSERT(target2[i]==source[i]);
        }
        block.free();
        total = cnv.save_block(block,target2);
        std::cerr << "\t"; Y_CHECK(total==block.size());
        {
            ios::imstream fp(block);
            total = cnv.load_block(target,fp);
            std::cerr << "\t"; Y_CHECK(total==block.size());
        }
        
    }
    
}


Y_UTEST(conveyors)
{
    ios::conveyors & IO = ios::conveyors::instance();
    
    const ios::conveyor &dch = IO.query<double>(comms::homogeneous);
    const ios::conveyor &dcd = IO.query<double>(comms::distributed);
    const ios::conveyor &sch = IO.query<string>(comms::homogeneous);
    const ios::conveyor &scd = IO.query<string>(comms::distributed);
    
    std::cerr << "dch=" << dch << std::endl;
    std::cerr << "dcd=" << dcd << std::endl;
    std::cerr << "sch=" << sch << std::endl;
    std::cerr << "scd=" << scd << std::endl;
    
    const ios::conveyor &cplxf_h = IO.query<complex,float>(comms::homogeneous);
    const ios::conveyor &cplxd_d = IO.query<complex,double>(comms::distributed);
    std::cerr << "cplxf_h=" << cplxf_h << std::endl;
    std::cerr << "cplxd_d=" << cplxd_d << std::endl;
    
    std::cerr << IO.query<point3d,double>(comms::homogeneous) << std::endl;
    
    IO.root().graphViz("convey.dot");
    
    
    
    IO.display();
    IO.sort();
    IO.display();
    
    IO.import(comms::homogeneous);
    IO.import(comms::distributed);

    IO.display();
    //IO.root().graphViz("convey-all.dot");
    
    Y_UTEST_SIZEOF(ios::conveyor);
    Y_UTEST_SIZEOF(ios::primary_conveyor<char>);
    Y_UTEST_SIZEOF(ios::primary_conveyor<int>);
    Y_UTEST_SIZEOF(ios::network_conveyor<char>);
    Y_UTEST_SIZEOF(ios::network_conveyor<int>);
    Y_UTEST_SIZEOF(ios::derived_conveyor<string>);
    Y_UTEST_SIZEOF(ios::derived_conveyor<mpn>);
    typedef ios::tuple_conveyor<complex,float,ios::primary_conveyor> complex_float_conveyer;
    Y_UTEST_SIZEOF(complex_float_conveyer);
    typedef ios::tuple_conveyor<point2d,double,ios::network_conveyor> point3d_double_conveyer;
    Y_UTEST_SIZEOF(point3d_double_conveyer);
    
    doTest<int>(    IO.query<int>(comms::homogeneous)    );
    doTest<int>(    IO.query<int>(comms::distributed)    );
    doTest<string>( IO.query<string>(comms::homogeneous) );
    doTest<string>( IO.query<string>(comms::distributed) );
    doTest< point2d<int>      >( IO.query<point2d,int>(comms::homogeneous) );
    doTest< point2d<int>      >( IO.query<point2d,int>(comms::distributed) );
    
    doTest< point3d<long long> >( IO.query<point3d,long long>(comms::homogeneous) );
    doTest< point3d<long long> >( IO.query<point3d,long long>(comms::distributed) );
    
    doTest<mpn>( IO.query<mpn>(comms::homogeneous) );
    doTest<mpn>( IO.query<mpn>(comms::distributed) );
    
    doTest<mpz>( IO.query<mpz>(comms::homogeneous) );
    doTest<mpz>( IO.query<mpz>(comms::distributed) );
    
    doTest<mpq>( IO.query<mpq>(comms::homogeneous) );
    doTest<mpq>( IO.query<mpq>(comms::distributed) );
    
    
}
Y_UTEST_DONE()

