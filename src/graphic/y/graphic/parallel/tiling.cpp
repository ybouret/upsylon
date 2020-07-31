

#include "y/graphic/parallel/tiling.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/parops.hpp"

namespace upsylon {

    namespace Graphic {

        namespace {

            static inline int byDecreasingProd( const Cores &lhs, const Cores &rhs ) throw()
            {
                const size_t nl = lhs.prod();
                const size_t nr = rhs.prod();
                return comparison::decreasing(nl,nr);
            }

            struct tileChooser
            {
                unit_t w,h;
                unit_t score( const Cores &cores ) const throw()
                {
                    return abs_of( unit_t(cores.x)*h - unit_t(cores.y)*w );
                }

                //! will sort by smaller score
                int operator()( const Cores &lhs, const Cores &rhs ) throw()
                {
                    return int(score(lhs)-score(rhs));
                }

            };
        }


        Cores Tiling:: ComputeCoresFor( const Area &area, const size_t CPUs)
        {
            static const char fn[] = "Graphic::Tiling::ComputeCoresFor: ";

            if(CPUs<=0)              throw exception("%sno CPUs",fn);
            if(area.w<=0||area.h<=0) throw exception("%sinvalid area",fn);

            const Cores maxCores( min_of<size_t>(CPUs,area.w), min_of<size_t>(CPUs,area.h) );

            Cores                        cores;
            vector<Cores,memory::pooled> probes( maxCores.prod(), as_capacity );

            for(cores.x=1;cores.x<=maxCores.x;++cores.x)
            {
                for(cores.y=1;cores.y<=maxCores.y;++cores.y)
                {
                    if(cores.prod()<=CPUs)
                    {
                        probes.push_back(cores);
                    }
                }
            }
            if(probes.size()<=0) throw exception("%sunexpected no cores!!!",fn);
            
            // select possible tilings
            hsort(probes,byDecreasingProd);
            const size_t ncpu = probes.front().prod();
            while( probes.back().prod() != ncpu ) probes.pop_back();

            // sort by decreasing score
            {
                tileChooser tch = { area.w, area.h };
                hsort(probes,tch);
            }


            return probes.front();
        }

        

    }
}
