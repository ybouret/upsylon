#ifndef Y_MATH_DIAG_SYMM_INCLUDED
#define Y_MATH_DIAG_SYMM_INCLUDED 1

#include "y/container/matrix.hpp"
#include "y/math/types.hpp"

namespace upsylon
{
    
    namespace math
    {
        struct diag_symm
        {
            
#define _Y_JACOBI(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau); a[k][l]=h+s*(g-h*tau)

            //! Jacobi reduction
            /**
             \param a is a symetric matrix, REGENERATED at the end...
             \param d are the eigenvalues
             \param v columns are the eigenvectors
             \param Diag = V'* A * V
             */
            template <typename T> static inline
            bool build( matrix<T> &a, array<T> &d, matrix<T> &v )
            {
                assert(a.rows>0);
                assert(a.is_square);
                assert(a.same_size_than(v));
                assert(d.size() == a.rows);
                
                bool           success = true;
                const size_t   n       = a.rows;
                array<T>      &b       = a.r_aux1;
                array<T>      &z       = a.r_aux2;

                //==============================================================
                // initialize eigenvectors and workspace
                //==============================================================
                for(size_t ip=1;ip<=n; ++ip)
                {
                    for(size_t iq=1;iq<=n;++iq) v[ip][iq]=T(0.0);
                    v[ip][ip]=T(1.0);
                    b[ip]=d[ip]=a[ip][ip];
                    z[ip]=T(0.0);
                }
                
                //==============================================================
                // looping over sweeps
                //==============================================================
                for(size_t iter=1;iter<=64;++iter)
                {
                    T sm = 0;
                    for(size_t ip=1;ip<n;++ip) {
                        for(size_t iq=ip+1;iq<=n; ++iq)
                            sm += __fabs(a[ip][iq]);
                    }
                    if (sm <= numeric<T>::minimum )
                    {
                        assert(true==success);
                        goto DONE; // OK
                    }
                    
                    const T tresh = (iter < 4) ? T(0.2)*sm/(n*n) : T(0.0);
                    
                    
                    for(size_t ip=1;ip<n;++ip)
                    {
                        for(size_t iq=ip+1;iq<=n;iq++)
                        {
                            T g=T(100) * __fabs(a[ip][iq]);
                            if (iter > 4 && almost_equal( __fabs(d[ip])+g, __fabs(d[ip]))
                                && almost_equal( __fabs(d[iq])+g, __fabs(d[iq])) )
                            {
                                a[ip][iq]=T(0.0);
                            }
                            else
                                if (__fabs(a[ip][iq]) > tresh)
                                {
                                    T h = d[iq]-d[ip];
                                    T t = 0;
                                    if ( almost_equal(__fabs(h)+g,__fabs(h)) )
                                    {
                                        t=(a[ip][iq])/h;
                                    }
                                    else
                                    {
                                        const T theta=T(0.5)*h/(a[ip][iq]);
                                        t=T(1.0)/(__fabs(theta)+__sqrt(T(1.0)+theta*theta));
                                        if (theta < T(0.0))
                                            t = -t;
                                    }
                                    const T c   = T(1.0)/__sqrt(T(1.0)+t*t);
                                    const T s   = t*c;
                                    const T tau = s/(T(1.0)+c);
                                    h=t*a[ip][iq];
                                    z[ip] -= h;
                                    z[iq] += h;
                                    d[ip] -= h;
                                    d[iq] += h;
                                    a[ip][iq]=T(0.0);
                                    for(size_t j=1;j<ip;++j) {
                                        _Y_JACOBI(a,j,ip,j,iq);
                                    }
                                    for(size_t j=ip+1;j<iq;++j) {
                                        _Y_JACOBI(a,ip,j,j,iq);
                                    }
                                    for(size_t j=iq+1;j<=n;++j) {
                                        _Y_JACOBI(a,ip,j,iq,j);
                                    }
                                    for(size_t j=1;j<=n;++j) {
                                        _Y_JACOBI(v,j,ip,j,iq);
                                    }
                                }
                        }
                    }
                    
                    for(size_t ip=1;ip<=n;++ip)
                    {
                        b[ip] += z[ip];
                        d[ip] = b[ip];
                        z[ip] = T(0.0);
                    }
                }
                success = false;
                
            DONE:
                //__________________________________________________________________
                //
                // matrix regeneration
                //__________________________________________________________________
                for(size_t i=1,ip=2;i<n;++i,++ip)
                {
                    for(size_t j=ip;j<=n;++j)
                    {
                        a[i][j] = a[j][i];
                    }
                }
                return success;
                
                
            }
        };
        
    }
    
}

#endif

