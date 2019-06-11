//! \file
#ifndef Y_MATH_KERNEL_TRIDIAG_INCLUDED
#define Y_MATH_KERNEL_TRIDIAG_INCLUDED 1

#include "y/sequence/arrays.hpp"
#include "y/math/types.hpp"
#include "y/type/xnumeric.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace math
    {

        template <typename T>
        class tridiag : public arrays<T>
        {
        public:
            typedef typename real_for<T>::type      scalar_type;

            virtual ~tridiag() throw() {}

            explicit tridiag(const size_t n) :
            arrays<T>(6,n),
            a( this->next() ),
            b( this->next() ),
            c( this->next() ),
            g( this->next() ),
            _r(this->next() ),
            _u(this->next() )
            {

            }

            inline T operator()(const size_t i, const size_t j) const throw()
            {
                assert(i>0); assert(i<=this->size());
                assert(j>0); assert(j<=this->size());
                if(i>j)
                {
                    if(1==i-j)
                    {
                        return a[i];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if( i<j )
                {
                    if(1==j-i)
                    {
                        return c[j-1];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return b[i];
                }
            }

            inline friend std::ostream & operator<<( std::ostream &os, const tridiag &tri )
            {
                os << '[';
                const size_t n = tri.size();
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=n;++j)
                    {
                        os << ' ' << tri(i,j);
                    }
                    if(i<n) os << ';';
                }
                os << ']';
                return os;
            }


            array<T> &a; //!< a[2]..a[size]
            array<T> &b; //!< b[1]..b[size]
            array<T> &c; //!< c[1]..c[size-1]


            inline void solve( array<T> &u, const array<T> &r )
            {
                assert(this->size()>0);
                assert(u.size()==this->size());
                assert(r.size()==this->size());

                // initialize
                const size_t      n    = this->size(); assert(n>0);
                const scalar_type amin = xnumeric<scalar_type>::abs_minimum();
                T piv = b[1];
                {
                    const scalar_type apiv = fabs_of(piv);
                    if(apiv<=amin) throw libc::exception(EDOM,"singular tridiag matrix-level 1");
                }

                // forward
                u[1]=r[1]/piv;
                for (size_t j=2;j<=n;j++)
                {
                    g[j]=c[j-1]/piv;
                    piv=b[j]-a[j]*g[j];
                    {
                        const scalar_type apiv = fabs_of(piv);
                        if(apiv<=amin)  throw libc::exception(EDOM,"singular tridiag matrix-level 2");
                    }
                    u[j]=(r[j]-a[j]*u[j-1])/piv;
                }

                // reverse
                for(size_t j=(n-1),jp=n;j>0;--j,--jp)
                {
                    u[j] -= g[jp]*u[jp];
                }
            }
            

            inline void mul( array<T> &target, const array<T> &source ) const
            {
                assert(target.size()==this->size());
                assert(source.size()==this->size());
                const size_t n = this->size();
                switch(n)
                {
                    case 0: return;
                    case 1: target[1] = b[1] * source[1]; return;
                    default:
                        assert(n>1);
                    {
                        const size_t nm1 = n-1;
                        target[1] = b[1] * source[1] + c[1] * source[2];
                        for(size_t i=nm1;i>1;--i)
                        {
                            target[i] = a[i] * source[i-1] + b[i] * source[i] + c[i] * source[i+1];
                        }
                        target[n] = a[n] * source[nm1] + b[n] * source[n];
                    }
                }
            }

        private:
            array<T> &g;
        public:
            array<T> &_r;
            array<T> &_u;
        };
    }
}

#endif

