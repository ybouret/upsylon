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
#if 0
                typedef Arc<T,VTX> ArcType;
                typedef typename   ArcType::PointType PointType;
                typedef typename   ArcType::vertex vertex;
                typedef typename   ArcType::Coefficients Coefficients;
                typedef typename   ArcType::type         type;
                static const size_t Dimensions = PointType::Dimensions;
                static inline void For(const PeriodicArc<T,VTX> &arc)
                {


                    aliasing::_(arc.compiled) = false;

                    const vertex zero;
                    const Nodes &nodes = arc.nodes;
                    const size_t size  = nodes.size();
                    Coefficients &P    = aliasing::_(arc.P);
                    Coefficients &A    = aliasing::_(arc.A);
                    P.make(size,zero);
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
                    vector<type> res(size,0);
                    M.set(1,4,1);
                    // collect
                    {
                        typename Nodes::const_iterator im = arc.nodes.begin();
                        typename Nodes::const_iterator i0 = im; ++i0;
                        typename Nodes::const_iterator ip = i0; ++ip;

                        // lower bound
                        {
                            fill(lhs,1,6,***arc.nodes.back(),****im,****i0);
                        }

                        // bulk
                        for(size_t i=2;i<size;++i,++im,++i0,++ip)
                        {
                            fill(lhs,i,6,****im,****i0,****ip);
                        }

                        // upper bound
                        fill(lhs,size,6,****im,****i0,***arc.nodes.front());
                        
                    }
                    std::cerr << "M=" << M << std::endl;
                    std::cerr << "lhs=" << lhs << std::endl;
                    for(size_t dim=0,row=1;dim<Dimensions;++dim,++row)
                    {
                        M.solve(res, lhs[row] );
                        std::cerr << "res" << dim << "=" << res << std::endl;
                        for(size_t i=size;i>0;--i)
                        {
                            type *v = (type *) &A[i];
                            v[dim]  = res[i];
                        }
                    }
                    std::cerr << "A=" << A << std::endl;
                    aliasing::_(arc.compiled) = true;

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
#endif
            };
        }

    }
}



#endif
