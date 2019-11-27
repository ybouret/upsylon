

#include "y/graphic/parallel/tiling.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"
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
                    return score(lhs)-score(rhs);
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


        void  Tiling:: ComputeTiles( const Area &area, const size_t CPUs )
        {
            const Cores  cores = ComputeCoresFor(area,CPUs);
            const size_t ncpus = cores.prod();

            std::cerr << "CPUs: " << CPUs << std::endl;
            std::cerr << "#Active Tiles: " << ncpus << std::endl;
            for(size_t ry=0;ry<cores.y;++ry)
            {
                unit_t h = area.h;
                unit_t j = 0;
                parops::split_any(h, j, cores.y, ry);

                for(size_t rx=0;rx<cores.x;++rx)
                {
                    unit_t w = area.w;
                    unit_t i = 0;
                    parops::split_any(w, i, cores.x,rx);
                    //std::cerr << "\t[" << w << "x" << h << "]->" << h*w << "@=(" << i << "," << j << ")" << std::endl;

                    const Point lower(i,j);
                    Point upper(i+w,j+h);
                    upper.dec();
                    //const Tile::Pointer tile = new Tile( lower, upper );
                    //std::cerr << "tile: " << tile << std::endl;
                }
                
            }

            std::cerr << "#Passive Tiles: " << CPUs-ncpus << std::endl;
            {
                const Point lo( 0, 0);
                const Point up(-1,-1);
                for(size_t n=ncpus;n<CPUs;++n)
                {
                    //const Tile::Pointer tile = new Tile(lo,up);
                    //std::cerr << "tile: " << tile << std::endl;
                }
            }
            std::cerr << std::endl;


        }


    }
}
