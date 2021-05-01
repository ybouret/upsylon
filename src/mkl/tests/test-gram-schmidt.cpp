#include "support.hpp"

#include "y/mkl/kernel/gram-schmidt.hpp"
#include "y/type/rtti.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/kernel/determinant.hpp"

using namespace upsylon;
using namespace mkl;

namespace {
    
    template <typename T>
    void doGS(const size_t maxDim=5)
    {
        std::cerr << "GramSchmidt<" << rtti::name_of<T>() << ">" << std::endl;
        for(size_t dim=1;dim<=maxDim;++dim)
        {
            std::cerr << "dim=" << dim << std::endl;
            matrix<T> a(dim,dim);
            a.diag(1,0);
            support::fill2D(a);
            //std::cerr << "a=" << a << std::endl;
            if(!GramSchmidt::Ortho(a))
            {
                std::cerr << "failure" << std::endl;
                continue;
            }
            //std::cerr << "->" << a << std::endl;
            for(size_t i=2;i<=a.rows;++i)
            {
                for(size_t k=1;k<i;++k)
                {
                    std::cerr << " " << tao::dot<T>::of(a[i],a[k]);
                }
                std::cerr << std::endl;
            }
        }
    }
    
    template <typename T>
    void doGSQ(const size_t maxDim=6)
    {
        std::cerr << std::endl;
        std::cerr << "-------- Special GramSchmidt<" << rtti::name_of<T>() << ">" << std::endl;
        for(size_t dim=1;dim<=maxDim;++dim)
        {
            std::cerr << "dim=" << dim << std::endl;
            matrix<T> a(dim,dim);
            a.diag(1,0);
            tao::ld(a[1], 1);
            std::cerr << "a=" << a << std::endl;
            if(!GramSchmidt::OrthoSimple(a))
            {
                std::cerr << "failure" << std::endl;
                continue;
            }
            std::cerr << "->" << a << std::endl;
            for(size_t i=2;i<=a.rows;++i)
            {
                for(size_t k=1;k<i;++k)
                {
                    std::cerr << " " << tao::dot<T>::of(a[i],a[k]);
                }
                std::cerr << std::endl;
            }
        }
    }

    template <typename T>
    void doGSI(const size_t maxDim=6)
    {
        std::cerr << std::endl;
        std::cerr << "-------- Integer GramSchmidt<" << rtti::name_of<T>() << ">" << std::endl;
        for(size_t dim=1;dim<=maxDim;++dim)
        {
            std::cerr << "dim=" << dim << std::endl;
            matrix<T> a(dim,dim);
            a.diag(1,0);
            tao::ld(a[1], 1);
            std::cerr << "a=" << a << std::endl;
            if(!GramSchmidt::iOrtho(a))
            {
                std::cerr << "failure" << std::endl;
                continue;
            }
            std::cerr << "->" << a << std::endl;
            for(size_t i=2;i<=a.rows;++i)
            {
                for(size_t k=1;k<i;++k)
                {
                    std::cerr << " " << tao::dot<T>::of(a[i],a[k]);
                }
                std::cerr << std::endl;
            }
        }
    }


    
}

Y_UTEST(gram_schmidt)
{

    doGS<float>();
    doGS<double>();
    doGS< complex<float> >();
    doGS< complex<double> >();
    doGS< apq >(4);
    
    doGSQ<apq>(6);
    doGSI<int16_t>(6);
    doGSI<int32_t>(6);
    doGSI<int64_t>(6);

    
}
Y_UTEST_DONE()

