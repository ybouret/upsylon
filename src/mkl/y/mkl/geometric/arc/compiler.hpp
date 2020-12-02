//! \file
#ifndef Y_MKL_GEOMETRIC_ARC_COMPILER_INCLUDED
#define Y_MKL_GEOMETRIC_ARC_COMPILER_INCLUDED 1

#include "y/mkl/geometric/arc/standard.hpp"
#include "y/mkl/geometric/arc/periodic.hpp"
#include "y/mkl/kernel/cyclic.hpp"

namespace upsylon
{
    namespace mkl
    {

        namespace Geometric
        {

            enum CompileArcLevel
            {
                Regular,
                Advanced
            };

            template <typename T, template <class> class VTX>
            struct CompileArc
            {
                typedef Arc<T,VTX> ArcType;
                typedef typename   ArcType::PointType PointType;
                typedef typename   ArcType::vertex vertex;
                typedef typename   ArcType::Coefficients Coefficients;
                typedef typename   ArcType::type         type;
                typedef typename   ArcType::Nodes        Nodes;
                static const size_t Dimensions = PointType::Dimensions;
                static inline void For(const PeriodicArc<T,VTX> &arc)
                {
                    const vertex zero;
                    const size_t size = arc.nodes.size();
                    Coefficients &A   = aliasing::_(arc.A);
                    A.make(size,zero);
                    switch(size)
                    {
                        case 0:
                        case 1:
                        case 2:
                            return;
                    }
                    cyclic<type> M(size);
                    matrix<type> lhs(Dimensions,size);
                    M.set(1,2,1);
                    // collect
                    {
                        typename Nodes::const_iterator it = arc.nodes.begin();
                        // lower bound
                        {
                            const vertex &p0 = ****it;
                            ++it;
                            const vertex &pp = ****it;
                            const vertex &pm = **arc.nodes.back();
                            fill(lhs,1,6,pm,p0,pp);
                        }

                        // bulk

                        // upper bound
                        
                    }
                }

                static inline void fill(matrix<type> &lhs,
                                        const size_t  col,
                                        const type    factor,
                                        const vertex &pm,
                                        const vertex &p0,
                                        const vertex &pp)
                {
                    const vertex d = factor * ( (pm-p0) + (pp-p0) );
                    const type  *v = (const type *)&d;
                    for(size_t r=1,i=0;i<Dimensions;++i,++r)
                    {
                        lhs[r][col] = v[i];
                    }
                }

            };
        }

    }
}



#endif
