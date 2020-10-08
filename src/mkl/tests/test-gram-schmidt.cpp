#include "support.hpp"

#include "y/mkl/kernel/gram-schmidt.hpp"
#include "y/type/spec.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/kernel/determinant.hpp"

using namespace upsylon;
using namespace mkl;

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

    template <typename T>
    void doGSSupp(const size_t maxDim)
    {
        std::cerr << std::endl;
        std::cerr << "-------- Integer GramSchmidt Supplementary<" << type_name_of<T>() << ">" << std::endl;
        for(size_t dim=2;dim<=maxDim;++dim)
        {
            std::cerr << "dim=" << dim << std::endl;
            for(size_t img=1;img<dim;++img)
            {
                const size_t ker=dim-img;
                std::cerr << "\timg=" << img << ", ker=" << ker << std::endl;
                matrix<T> a(img,dim);
                matrix<T> q(ker,dim);
                matrix<T> a2(img,img);
                a.diag(1,0);
                do
                {
                    for(size_t i=img;i>0;--i)
                    {
                        for(size_t j=dim;j>0;--j)
                        {
                            a[i][j] = alea.range<int>(-5,5);
                        }
                    }
                    quark::mmul_rtrn(a2,a,a);
                }
                while( 0 == ideterminant(a2) );

                std::cerr << "a=" << a << std::endl;
                {
                    matrix<T> b = a;
                    if(!GramSchmidt::iOrtho(b))
                    {
                        throw exception("no ortho for free family!");
                    }
                    std::cerr << "b=" << b << std::endl;
                }

                if(GramSchmidt::iOrthoRows(q,a))
                {
                    std::cerr << "q=" << q << std::endl;
                }
                else
                {
                    std::cerr << "couldn't find supp" << std::endl;
                }
            }
            std::cerr << std::endl;
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

    doGSSupp<int64_t>(6);

}
Y_UTEST_DONE()

