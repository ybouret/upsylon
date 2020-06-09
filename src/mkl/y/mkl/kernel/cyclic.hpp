//! \file
#ifndef Y_MKL_KERNEL_CYCLIC_INCLUDED
#define Y_MKL_KERNEL_CYCLIC_INCLUDED 1

#include "y/mkl/kernel/tridiag.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    namespace math
    {

        //! 3-cyclic system, using Woodbury
        template <typename T>
        class cyclic : public tridiag<T>
        {
        public:
            Y_DECL_ARGS(T,type);                                 //!< alias
            typedef typename arrays<T>::array_type  array_type;  //!< alias
            typedef typename real_for<T>::type      scalar_type; //!< alias
            static const  size_t                    space = 2;   //!< number of extra values

            //! cleanup
            inline virtual ~cyclic() throw() {}

            //! allocated resources
            inline cyclic(const size_t n, const size_t extra=0) :
            tridiag<T>(check_size(n),extra),
            U(space,n),
            V(space,n),
            Z(space,n),
            H(space,space),
            one(1)
            {
                U[1][1] = one;
                U[2][n] = one;
            }

            //! set cyclic A,B,C
            inline void set( param_type A, param_type B, param_type C )
            {
                assert(this->size()>=3);
                for(size_t i=this->size();i>0;--i)
                {
                    this->a[i] = A;
                    this->b[i] = B;
                    this->c[i] = C;
                }
            }


            //! solve (*this)*x = r
            inline void solve( array<T> &x, const array<T> &r )
            {
                assert( this->size() == x.size() );
                assert( this->size() == r.size() );
                assert(this->size()>=3);
                
                // setup V
                const size_t n = this->size();
                V[1][n] = this->a[1];
                V[2][1] = this->c[n];

                // compute Z
                for(size_t i=space;i>0;--i)
                {
                    addressable<T> &Zi = Z[i];
                    tridiag<T>::solve(Zi,U[i]);
                }

                // compute H = inv(1+V'*Z)
                for(size_t i=space;i>0;--i)
                {
                    addressable<T>      &Hi = H[i];
                    const accessible<T> &Vi = V[i];
                    for(size_t j=space;j>0;--j)
                    {
                        type ans = 0;
                        {
                            const accessible<T> &Zj = Z[j];
                            for(size_t k=n;k>0;--k)
                            {
                                ans += Vi[k] * Zj[k];
                            }
                        }
                        Hi[j] = ans;
                    }
                    Hi[i] += one;
                }

                if( !LU::build(H) )
                {
                    throw libc::exception(EDOM,"singular cyclic system");
                }

                // raw solution in x
                tridiag<T>::solve(x,r);

                // compute H * V' * x
                array<T> &HVx = H.c_aux2;
                for(size_t i=space;i>0;--i)
                {
                    const accessible<T> &Vi = V[i];
                    type ans = 0;
                    for(size_t k=n;k>0;--k)
                    {
                        ans += Vi[k] * x[k];
                    }
                    HVx[i] = ans;
                }
                LU::solve(H,HVx);

                // substact Z * (H * V' * x )
                for(size_t k=n;k>0;--k)
                {
                    for(size_t j=space;j>0;--j)
                    {
                        x[k] -= Z[j][k] * HVx[j];
                    }
                }
            }

            //! get value at row i, column j
            inline T operator()(const size_t i, const size_t j) const throw()
            {
                assert(i>0); assert(i<=this->size());
                assert(j>0); assert(j<=this->size());
                const size_t n = this->size();
                if(i==1)
                {
                    //__________________________________________________________
                    //
                    // first row
                    //__________________________________________________________
                    if(j==1)
                    {
                        return this->b[1];
                    }
                    else if(j==2)
                    {
                        return this->c[1];
                    }
                    else if(j==n)
                    {
                        return this->a[1];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if(i==n)
                {
                    //__________________________________________________________
                    //
                    // last row
                    //__________________________________________________________
                    if(j==1)
                    {
                        return this->c[n];
                    }
                    else if(j==n)
                    {
                        return this->b[n];
                    }
                    else if(j==n-1)
                    {
                        return this->a[n];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    //__________________________________________________________
                    //
                    // bulk
                    //__________________________________________________________
                    if(i>j)
                    {
                        if(1==i-j)
                        {
                            return this->a[i];
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
                            return this->c[j-1];
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return this->b[i];
                    }
                }

            }

            //! display
            inline friend std::ostream & operator<<( std::ostream &os, const cyclic &cc )
            {
                os << '[';
                const size_t n = cc.size();
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=n;++j)
                    {
                        os << ' ' << cc(i,j);
                    }
                    if(i<n) os << ';';
                }
                os << ']';
                return os;
            }

            //! target = (*this)*source
            inline void mul( array<T> &target, const array<T> &source ) const
            {
                assert(this->size()>=3);
                assert(target.size()==this->size());
                assert(source.size()==this->size());
                const size_t n = this->size();

                // first row
                target[1] = this->b[1] * source[1] + this->c[1] * source[2] + this->a[1] * source[n];

                // last row
                target[n] = this->a[n] * source[n-1] + this->b[n] * source[n] + this->c[n] * source[1];

                // bulk
                for(size_t i=2;i<n;++i)
                {
                    target[i] = this->a[i] * source[i-1] + this->b[i] * source[i] + this->c[i] * source[i+1];
                }

            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(cyclic);
            matrix<T>  U;   //!< matrix of column position, made of ones
            matrix<T>  V;   //!< matrix of rows position+values
            matrix<T>  Z;   //!< local problems
            matrix<T>  H;   //!< dispatch matrix
            const_type one; //!< precomputed 1

            static inline size_t check_size(const size_t n)
            {
                if(n<3) throw libc::exception(EDOM,"cyclic is too small");
                return n;
            }
        };

    }
}

#endif

