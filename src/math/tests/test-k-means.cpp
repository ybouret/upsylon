#include "y/math/stat/k-means.hpp"
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
        vector<VERTEX> vertices;
        for(size_t i=100+alea.leq(100);i>0;--i)
        {
            const VERTEX v = support::get<VERTEX>();
            vertices.push_back(v);
        }
        
        typedef k_means<T,VERTEX>         kMeans;
        typedef typename kMeans::centroid centroid;
        vector<centroid,memory::pooled>   centroids;
        
        for(size_t i=3;i>0;--i)
        {
            const VERTEX v = support::get<VERTEX>();
            const centroid c = new typename kMeans::centroid_(v);
            centroids.push_back(c);
            Y_ASSERT( memcmp( &(c->v), &(centroids.back()->v), sizeof(VERTEX)) == 0);
        }
        
        std::cerr << "centroids: " << centroids << std::endl;
        vector<size_t> indices(vertices.size(),0);
        
        bool success = true;
        for(size_t iter=1;iter<=16;++iter)
        {
            for(size_t i=1;i<=vertices.size();++i)
            {
                const size_t ic = kMeans::find_closest(centroids,vertices[i]);
                indices[i] = ic;
            }
            
            std::cerr << "indices=" << indices << std::endl;
            if(!kMeans::update(centroids,vertices,indices))
            {
                success = false;
                break;
            }
            std::cerr << "centroids: " << centroids << std::endl;
        }
        
        for(size_t i=1;i<=vertices.size();++i)
        {
            const size_t ic = kMeans::find_closest(centroids,vertices[i]);
            indices[i] = ic;
        }
        
        
        {
            const string id = type_spec::to_file_name(type_name_of<VERTEX>()) + "-data.dat";
            std::cerr << "->" << id << std::endl;
            ios::ocstream fp(id);
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
                const double *p = (const double*)&(centroids[ic]->v);
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
    doTest<double,double>();
    doTest<float,point2d<float> >();
    doTest<float,point3d<float> >();
    doTest<short,short>();
    
}
Y_UTEST_DONE()

