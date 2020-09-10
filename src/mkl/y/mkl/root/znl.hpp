
//! \file

#ifndef Y_MATH_FCN_ZNL_INCLUDED
#define Y_MATH_FCN_ZNL_INCLUDED 1

#include "y/mkl/fcn/jacobian.hpp"
#include "y/mkl/kernel/svd.hpp"
#include "y/mkl/utils.hpp"
#include "y/sequence/arrays.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/core/temporary-link.hpp"
#include "y/core/temporary-value.hpp"
#include "y/comparison.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace kernel
        {
            class znl
            {
            public:

            private:
                
            };
        }


        template <typename T>
        class znl
        {

        public:
            typedef typename numeric<T>::vector_field field_type; //!< alias
            typedef typename numeric<T>::function     function1d; //!< alias
            typedef lightweight_array<T>              array_type; //!< alias


            inline explicit znl() :
            nvar(0),
            A(8),
            J(),
            u(),
            v(),
            w( A.next() ),
            f_(0),
            X_(0)
            {
            }

            inline virtual ~znl() throw()
            {
            }

            //! starting with f(F,X) precomputed
            bool cycle( addressable<T> &F, addressable<T> &X, field_type &f, jacobian<T> &fjac )
            {
                assert( F.size() == X.size() );
                //--------------------------------------------------------------
                // prepare topology
                //--------------------------------------------------------------
                core::temporary_value<size_t>         nlink(nvar,X.size());
                core::temporary_link<field_type>      flink(f,&f_);
                core::temporary_link<addressable<T> > xlink(X,&X_);

                A.acquire(nvar);
                J.make(nvar,nvar);
                u.make(nvar,nvar);
                v.make(nvar,nvar);

                fjac(J,X);
                u.assign(J);
                if(!svd::build(u,w,v))
                {
                    return false;
                }

                //--------------------------------------------------------------
                // study solutions
                //--------------------------------------------------------------
                const size_t ker = __find<T>::truncate(w);


                return false;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(znl);
            size_t           nvar;
            arrays<T>        A;
            matrix<T>        J;
            matrix<T>        u;
            matrix<T>        v;
            array_type      &w;
            field_type      *f_;
            addressable<T>  *X_;


        };

    }

}

#endif
