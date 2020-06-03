//! \file

#ifndef Y_SPADE_BOX_INCLUDED
#define Y_SPADE_BOX_INCLUDED 1

#include "y/spade/vertices.hpp"

namespace upsylon {

    namespace Spade {

        namespace Kernel {

            //! generic Box type in 1|2|3 Dimensions
            template <typename T, unsigned DIMS>
            class Box
            {
            public:
                Y_DECL_ARGS(T,type);                                           //!< aliases
                static const unsigned Dimensions = DIMS;                       //!< alias
                typedef typename VertexIn<DIMS>:: template Of<T>::Type Vertex; //!< alias

                //! cleanup
                inline virtual ~Box() throw()
                {
                    _bzset(lower);
                    _bzset(upper);
                    _bzset(width);
                    _bzset(space);
                }

                //! setup with order rearrangement and space computation
                inline explicit Box(const Vertex lo, const Vertex up) throw() :
                lower(lo),
                upper(up),
                width(up-lo),
                space(1)
                {
                    mutable_type *l = (mutable_type *)&lower;
                    mutable_type *u = (mutable_type *)&upper;
                    mutable_type *w = (mutable_type *)&width;
                    for(unsigned dim=0;dim<Dimensions;++dim)
                    {
                        mutable_type &L = l[dim];
                        mutable_type &U = u[dim];
                        if(U<L) cswap(L,U);
                        assert(L<=U);
                        aliasing::_(space) *= ( w[dim] = U-L );
                    }
                }

                //! copy
                inline Box(const Box &box) throw() :
                lower(box.lower),
                upper(box.upper),
                width(box.width),
                space(box.space)
                {

                }

                //! helper
                inline const_type * _lower() const throw() { return (const_type *)&lower; }

                //! helper
                inline const_type * _upper() const throw() { return (const_type *)&upper; }

                //! helpe
                inline const_type * _width() const throw() { return (const_type *)&width; }


                const Vertex lower; //!< lower vertex
                const Vertex upper; //!< upper vertex
                const Vertex width; //!< upper - lower
                const_type   space; //!< length|surface|volume

                //! display
                inline friend std::ostream & operator<<(std::ostream &os, const Box &box)
                {
                    os << "((" << box.lower << "->" << box.upper << ":|" << box.width << "|=" << box.space << "))";
                    return os;
                }

            private:
                Y_DISABLE_ASSIGN(Box);
            };

        }
    }
}

#endif
