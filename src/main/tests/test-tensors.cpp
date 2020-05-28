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
                std::cerr << "1D..." << std::endl;
                tensor1d<T> t1( 1+alea.leq(10) );
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
                std::cerr << "2D..." << std::endl;
                tensor2d<T> t2( 1+alea.leq(10), 1+alea.leq(10) );
                //support::fill2D(t2);
                std::cerr << "tensor2d<" << id << ">.bytes=" << t2.allocated() << std::endl;
                
                collect.free();
                for(size_t i=1;i<=t2.rows;++i)
                {
                    for(size_t j=1;j<=t2.cols;++j)
                    {
                        const T tmp = support::get<T>();
                        collect.push_back(tmp);
                        t2[i][j] = tmp;
                    }
                }
                
                const tensor2d<T> &k2 = t2;
                
                for(size_t i=1;i<=t2.rows;++i)
                {
                    for(size_t j=1;j<=t2.cols;++j)
                    {
                        Y_ASSERT( collect.front() == k2[i][j] );
                        collect.pop_front();
                    }
                }
                
            }
            
            
            {
                std::cerr << "3D..." << std::endl;
                tensor3d<T> t3( 1+alea.leq(10), 1+alea.leq(10), 1+alea.leq(10) );
                std::cerr << "tensor3d<" << id << ">.bytes=" << t3.allocated() << std::endl;
                std::cerr << "slices=" << t3.slices << std::endl;
                collect.free();
                for(size_t i=1;i<=t3.rows;++i)
                {
                    for(size_t j=1;j<=t3.cols;++j)
                    {
                        for(size_t k=1;k<=t3.slices;++k)
                        {
                            const T tmp = support::get<T>();
                            collect.push_back(tmp);
                            t3[i][j][k] = tmp;
                        }
                    }
                }
                continue;
                
                const tensor3d<T> &k3 = t3;
                for(size_t i=1;i<=t3.rows;++i)
                {
                    for(size_t j=1;j<=t3.cols;++j)
                    {
                        for(size_t k=1;k<=t3.slices;++k)
                        {
                            Y_ASSERT( collect.front() == k3[i][j][k] );
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
    doTest<mpn>();
    doTest<string>();
    
}
Y_UTEST_DONE()

