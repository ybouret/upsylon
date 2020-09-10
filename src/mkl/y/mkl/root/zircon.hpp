
//! \file

#ifndef Y_MATH_FCN_ZIRCON_INCLUDED
#define Y_MATH_FCN_ZIRCON_INCLUDED 1

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
            class zircon
            {
            public:
                static const char CLID[];
                bool              verbose;

                virtual ~zircon() throw();

            protected:
                explicit zircon() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(zircon);
            };
        }

#define Y_ZIRCON_PRINTLN(MSG) \
do { if(this->verbose) { std::cerr << '[' << CLID << ']' << MSG << std::endl; } } while(false)

        template <typename T>
        class zircon : public kernel::zircon
        {

        public:
            typedef typename numeric<T>::vector_field ftype; //!< alias
            typedef typename jacobian<T>::type        jtype; //!, alias
            typedef typename numeric<T>::function     function1d; //!< alias
            typedef lightweight_array<T>              array_type; //!< alias


            inline explicit zircon() :
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

            inline virtual ~zircon() throw()
            {
            }

            //! starting with f(F,X) precomputed
            bool cycle( addressable<T> &F, addressable<T> &X, ftype &f, jtype &fjac )
            {
                assert( F.size() == X.size() );
                //--------------------------------------------------------------
                // prepare topology
                //--------------------------------------------------------------
                core::temporary_value<size_t>         nlink(nvar,X.size());
                core::temporary_link<ftype>           flink(f,&f_);
                core::temporary_link<addressable<T> > xlink(X,&X_);

                Y_ZIRCON_PRINTLN("<cycle>");
                Y_ZIRCON_PRINTLN("X="<<X);
                Y_ZIRCON_PRINTLN("F="<<F);

                A.acquire(nvar);
                J.make(nvar,nvar);
                u.make(nvar,nvar);
                v.make(nvar,nvar);

                fjac(J,X);
                u.assign(J);
                if(!svd::build(u,w,v))
                {
                    Y_ZIRCON_PRINTLN("svd failure");
                    return false;
                }

                //--------------------------------------------------------------
                // study solutions
                //--------------------------------------------------------------
                const size_t ker = __find<T>::truncate(*w,nvar);
                Y_ZIRCON_PRINTLN("#ker="<<ker);
                if(ker>=nvar)
                {
                    Y_ZIRCON_PRINTLN("singular matrix");
                    return false;
                }

                return false;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(zircon);
            size_t           nvar;
            arrays<T>        A;
            matrix<T>        J;
            matrix<T>        u;
            matrix<T>        v;
            array_type      &w;
            ftype           *f_;
            addressable<T>  *X_;


        };

    }

}

#endif
