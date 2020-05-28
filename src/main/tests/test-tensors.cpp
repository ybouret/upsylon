#include "y/tensor/tensor3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;


namespace {
    
    template <typename T>
    static inline void doTest()
    {
        std::cerr << std::endl;
        const string &id = type_name_of<T>();
        
        list<T>     collect;
        for(size_t iter=0;iter<10;++iter)
        {
            {
                tensor1d<T> t1( 1+alea.leq(100) );
                //support::fill1D(t1);
                std::cerr << "tensor1d<" << id << ">.bytes=" << t1.allocated() << std::endl;
                
                collect.free();
                for(size_t i=1;i<=t1.cols;++i)
                {
                    const T tmp = support::get<T>();
                    collect.push_back(tmp);
                    t1[i] = tmp;
                }
                
                const tensor1d<T> &k1 = t1;
                for(size_t i=1;i<=t1.cols;++i)
                {
                    Y_ASSERT( collect.front() == k1[i] );
                    collect.pop_front();
                }
                
            }
            
            {
                tensor2d<T> t2( 1+alea.leq(100), 1+alea.leq(100) );
                std::cerr << "tensor2d<" << id << ">.bytes=" << t2.allocated() << std::endl;
                
                collect.free();
                for(size_t j=1;j<=t2.rows;++j)
                {
                    for(size_t i=1;i<=t2.cols;++i)
                    {
                        const T tmp = support::get<T>();
                        collect.push_back(tmp);
                        t2[j][i] = tmp;
                    }
                }
                
                
                const tensor2d<T> &k2 = t2;
                
                for(size_t j=1;j<=t2.rows;++j)
                {
                    for(size_t i=1;i<=t2.cols;++i)
                    {
                        Y_ASSERT( collect.front() == k2[j][i] );
                        collect.pop_front();
                    }
                }
                
            }
            
            
            {
                tensor3d<T> t3( 1+alea.leq(30), 1+alea.leq(30), 1+alea.leq(30) );
                std::cerr << "tensor3d<" << id << ">.bytes=" << t3.allocated() << std::endl;
                collect.free();
                
                for(size_t k=1;k<=t3.slices;++k)
                {
                    for(size_t j=1;j<=t3.rows;++j)
                    {
                        for(size_t i=1;i<=t3.cols;++i)
                        {
                            const T tmp = support::get<T>();
                            collect.push_back(tmp);
                            t3[k][j][i] = tmp;
                        }
                    }
                }
                
                const tensor3d<T> &k3 = t3;
                for(size_t k=1;k<=t3.slices;++k)
                {
                    for(size_t j=1;j<=t3.rows;++j)
                    {
                        for(size_t i=1;i<=t3.cols;++i)
                        {
                            Y_ASSERT( collect.front() == k3[k][j][i] );
                            collect.pop_front();
                        }
                        
                    }
                }
            }
        }
        
    }
    
}


Y_UTEST(tensors)
{
    doTest<int>();
    doTest<uint8_t>();
    doTest<string>();
    doTest<mpn>();

}
Y_UTEST_DONE()

