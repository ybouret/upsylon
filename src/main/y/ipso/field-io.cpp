#include "y/ipso/field-io.hpp"

namespace upsylon
{
    namespace ipso
    {
        template <>
        void field_io:: collect<layout1D>(sequence<coord1D> &indices,
                                          const layout1D    &full,
                                          const layout1D    &sub )
        {
            assert( full.contains(sub) );
            for(coord1D i=sub.lower;i<=sub.upper;++i)
            {
                indices.push_back( full.index_of(i) );
            }
        }

        template <>
        void field_io:: collect<layout2D>(sequence<coord1D> &indices,
                                          const layout2D    &full,
                                          const layout2D    &sub )
        {
            assert( full.contains(sub) );
            for( coord1D j=sub.lower.y; j<=sub.upper.y;++j)
            {
                for(coord1D i=sub.lower.x;i<=sub.upper.x;++i)
                {
                    const coord2D q(i,j);
                    indices.push_back( full.index_of(q) );
                }
            }
        }

        template <>
        void field_io:: collect<layout3D>(sequence<coord1D> &indices,
                                          const layout3D    &full,
                                          const layout3D    &sub )
        {
            assert( full.contains(sub) );
            for( coord1D k=sub.lower.z; k<=sub.upper.z;++k)
            {
                for( coord1D j=sub.lower.y; j<=sub.upper.y;++j)
                {
                    for(coord1D i=sub.lower.x;i<=sub.upper.x;++i)
                    {
                        const coord3D q(i,j,k);
                        indices.push_back( full.index_of(q) );
                    }
                }
            }
        }

    }

}

