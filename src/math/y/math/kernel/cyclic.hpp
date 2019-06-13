//! \file
#ifndef Y_MATH_KERNEL_CYCLIC_INCLUDED
#define Y_MATH_KERNEL_CYCLIC_INCLUDED 1

#include "y/math/kernel/tridiag.hpp"


namespace upsylon
{
    namespace math
    {

        class cyclic_info
        {
        public:
            static const size_t cyclic_reserved = 3;
            const size_t        cyclic_offset;
            const size_t        cyclic_extraneous;

            virtual ~cyclic_info() throw();

        protected:
            explicit cyclic_info(const size_t internal, const size_t avail) throw();

            static size_t check_size(const size_t n); //!< n>=3

        private:
            Y_DISABLE_COPY_AND_ASSIGN(cyclic_info);
        };

        template <typename T>
        class cyclic :  public tridiag<T>, public cyclic_info
        {
        public:
            Y_DECL_ARGS(T,type);

            typedef typename arrays<T>::array_type  array_type;  //!< alias
            typedef typename real_for<T>::type      scalar_type; //!< alias

            inline virtual ~cyclic() throw() {}

            inline cyclic(const size_t n, const size_t extra=0) :
            tridiag<T>(check_size(n),cyclic_reserved+extra),
            cyclic_info(this->tridiag_reserved+cyclic_reserved,extra),
            bb( this->next() ),
            u(  this->next() ),
            z(  this->next() ),
            one(1)
            {
            }

            inline void set( param_type A, param_type B, param_type C )
            {
                for(size_t i=this->size();i>0;--i)
                {
                    this->a[i] = A;
                    this->b[i] = B;
                    this->c[i] = C;
                }
            }

            //! get array [0..extraneous-1]
            inline virtual const array_type & get_array(const size_t i) const throw()
            {
                assert(i<cyclic_extraneous);
                return this->raw_get(i+cyclic_offset);
            }

            //! solve (*this)*x = r
            inline void solve( array<T> &x, const array<T> &r )
            {
                assert( this->size() == x.size() );
                assert( this->size() == r.size() );

                const scalar_type amin  = xnumeric<scalar_type>::abs_minimum();
                const size_t      n     = this->size();
                const_type       &alpha = this->c[n];
                const_type       &beta  = this->a[1];

                const_type gamma = - this->b[1];
                {
                    const scalar_type agam = fabs_of(gamma);
                    if(agam<=amin) throw libc::exception(EDOM,"singular cyclic system");
                }

                bb[1]= this->b[1]-gamma;
                bb[n]= this->b[n]-alpha*beta/gamma;
                for(size_t i=2;i<n;i++) bb[i]=this->b[i];
                tridiag<T>::solve(x,r);
                u[1]=gamma;
                u[n]=alpha;
                for(size_t i=2;i<n;i++) u[i]=0;
                tridiag<T>::solve(z,u);
                const T fact=(x[1]+beta*x[n]/gamma)/
                (one+z[1]+beta*z[n]/gamma);
                for(size_t i=n;i>0;--i) x[i] -= fact*z[i];
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
                        return this->a[j];
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
                std::cerr << "cyclic mul" << std::endl;
                assert(target.size()==this->size());
                assert(source.size()==this->size());
                const size_t n = this->size();

                // first row
                target[1] = this->b[1] * source[1] + this->c[1] * source[2] + this->a[n] * source[n];

                // last row
                target[n] = this->a[n-1] * source[n-1] + this->b[n] * source[n] + this->c[n] * source[1];

                // bulk
                for(size_t i=2;i<n;++i)
                {
                    target[i] = this->a[i] * source[i-1] + this->b[i]  * source[i] + this->c[i] * source[i+1];
                }

            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(cyclic);
            array_type &bb;
            array_type &u;
            array_type &z;
            const T     one;
        };

    }
}

#endif

