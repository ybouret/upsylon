#include "support.hpp"

#include "y/math/kernel/gram-schmidt.hpp"
#include "y/type/spec.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;

namespace {
    
    template <typename T>
    void doGS(const size_t maxDim=5)
    {
        std::cerr << "GramSchmidt<" << type_name_of<T>() << ">" << std::endl;
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
                    std::cerr << " " << quark::dot<T>::of(a[i],a[k]);
                }
                std::cerr << std::endl;
            }
        }
    }
    
    template <typename T>
    void doGSQ(const size_t maxDim=6)
    {
        std::cerr << std::endl;
        std::cerr << "-------- Special GramSchmidt<" << type_name_of<T>() << ">" << std::endl;
        for(size_t dim=1;dim<=maxDim;++dim)
        {
            std::cerr << "dim=" << dim << std::endl;
            matrix<T> a(dim,dim);
            a.diag(1,0);
            quark::ld(a[1], 1);
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
                    std::cerr << " " << quark::dot<T>::of(a[i],a[k]);
                }
                std::cerr << std::endl;
            }
        }
    }

    template <typename T>
    void doGSI(const size_t maxDim=6)
    {
        std::cerr << std::endl;
        std::cerr << "-------- Integer GramSchmidt<" << type_name_of<T>() << ">" << std::endl;
        for(size_t dim=1;dim<=maxDim;++dim)
        {
            std::cerr << "dim=" << dim << std::endl;
            matrix<T> a(dim,dim);
            a.diag(1,0);
            quark::ld(a[1], 1);
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
                    std::cerr << " " << quark::dot<T>::of(a[i],a[k]);
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
    doGS< mpq >(4);
    
    doGSQ<mpq>(6);
    doGSI<int16_t>(6);
    doGSI<int32_t>(6);
    doGSI<int64_t>(6);

}
Y_UTEST_DONE()

