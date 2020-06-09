#include "y/mkl/stat/k-means.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;

namespace {
    
    template <typename T,typename VERTEX>
    void doTest()
    {
        std::cerr << std::endl;
        std::cerr << "k_means<" << type_name_of<T>() << "," << type_name_of<VERTEX>() << ">" << std::endl;
        vector<VERTEX> vertices;
        for(size_t i=100+alea.leq(100);i>0;--i)
        {
            const VERTEX v = support::get<VERTEX>();
            vertices.push_back(v);
        }
        
        typedef k_means<T,VERTEX>         kMeans;
        typedef typename kMeans::centroid centroid;
        vector<centroid,memory::pooled>   centroids;
        vector<centroid>                  centroids2;
        
        for(size_t i=1+alea.leq(5);i>0;--i)
        {
            const VERTEX v = support::get<VERTEX>();
            const centroid tmp( (const T*)&v);
            centroids.push_back(tmp);
            centroids2.push_back(tmp);
            Y_ASSERT( memcmp(centroids.back().r,  tmp.r, kMeans::dimensions*sizeof(double))==0 );
            Y_ASSERT( memcmp(centroids2.back().r, tmp.r, kMeans::dimensions*sizeof(double))==0 );
            
        }
        
        std::cerr << "centroids: " << centroids << std::endl;
        const size_t nv = vertices.size();
        
        vector<size_t> indices(nv,0);
        vector<size_t> indices2(nv,0);
        vector<size_t> weights(nv,0);
        for(size_t i=1;i<=nv;++i) weights[i] = 1 + alea.leq(10);
        
        {
            bool success = true;
            while(true)
            {
                if(kMeans::built(centroids,vertices,indices))
                {
                    std::cerr << "\tachieved!" << std::endl;
                    for(size_t i=vertices.size();i>0;--i)
                    {
                        Y_ASSERT( indices[i] == kMeans::find_closest(centroids,vertices[i]));
                    }
                    break;
                }
                
                if(!kMeans::update(centroids))
                {
                    success = false;
                    break;
                }
                std::cerr << "centroids: " << centroids << std::endl;
            }
            const string id = type_spec::to_file_name(type_name_of<VERTEX>()) + "-data.dat";
            ios::ocstream fp(id);
            if(success)
            {
                std::cerr << "\tsuccess!" << std::endl;
            }
            else
            {
                std::cerr << "\tfailure!" << std::endl;
            }
            
            for(size_t i=1;i<=vertices.size();++i)
            {
                {
                    const T *p = (const T*)&vertices[i];
                    fp("%g", double(p[0]));
                    for(size_t j=1;j<kMeans::dimensions;++j)
                    {
                        fp(" %g", double(p[j]));
                    }
                }
                const unsigned ic = unsigned(indices[i]);
                fp(" %u", ic );
                fp << "\n";
                const double *p = centroids[ic].r;
                fp("%g", double(p[0]));
                for(size_t j=1;j<kMeans::dimensions;++j)
                {
                    fp(" %g", double(p[j]));
                }
                fp(" %u",ic );
                fp << "\n";
                
                fp << "\n";
            }
        }
        
        //std::cerr << "w: " << weights << std::endl;
        
        {
            bool success = true;
            while(true)
            {
                if(kMeans::built(centroids2,vertices,indices2,weights))
                {
                    std::cerr << "\tachieved!" << std::endl;
                    break;
                }
                
                if(!kMeans::update(centroids2))
                {
                    success = false;
                    break;
                }
                std::cerr << "centroids2: " << centroids2 << std::endl;
            }
            const string id = type_spec::to_file_name(type_name_of<VERTEX>()) + "-data2.dat";
            ios::ocstream fp(id);
            if(success)
            {
                std::cerr << "\tsuccess!" << std::endl;
            }
            else
            {
                std::cerr << "\tfailure!" << std::endl;
            }
            
            for(size_t i=1;i<=vertices.size();++i)
            {
                {
                    const T *p = (const T*)&vertices[i];
                    fp("%g", double(p[0]));
                    for(size_t j=1;j<kMeans::dimensions;++j)
                    {
                        fp(" %g", double(p[j]));
                    }
                }
                const unsigned ic = unsigned(indices[i]);
                fp(" %u", ic );
                fp << "\n";
                const double *p = centroids2[ic].r;
                fp("%g", double(p[0]));
                for(size_t j=1;j<kMeans::dimensions;++j)
                {
                    fp(" %g", double(p[j]));
                }
                fp(" %u",ic );
                fp << "\n";
                
                fp << "\n";
            }
        }
        
        
        
        
    }
    // plot 'point2d-float--data.dat' w lp lc var
}

Y_UTEST(k_means)
{
    kernel::centroid<1> c1; std::cerr << c1 << std::endl;
    kernel::centroid<2> c2; std::cerr << c2 << std::endl;
    kernel::centroid<3> c3; std::cerr << c3 << std::endl;
    
    doTest<double,double>();
    doTest<float,point2d<float> >();
    doTest<float,point3d<float> >();
    doTest<short,short>();
}
Y_UTEST_DONE()

