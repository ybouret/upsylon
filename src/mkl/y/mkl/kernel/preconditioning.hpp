//! \file

#ifndef Y_ML_PRECOND_INCLUDED
#define Y_ML_PRECOND_INCLUDED 1


#include "y/container/matrix.hpp"
#include "y/sequence/arrays.hpp"
#include "y/sort/heap.hpp"
#include "y/core/temporary-link.hpp"
#include "y/core/temporary-value.hpp"
#include "y/core/temporary-acquire.hpp"
#include "y/type/aliasing.hpp"
#include "y/mkl/utils.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace upsylon
{
    namespace mkl
    {

        //! preconditioning matrices
        template <typename T>
        class preconditioning
        {
        public:
            typedef lightweight_array<T>            array_type; //!< alias
            typedef typename numeric<T>::function   function1d; //!< alias

            //! setup
            inline explicit preconditioning() :
            nvar(0),
            dmin(0),
            dmax(0),
            drho(0),
            diag_(0),
            wksp_(0),
            delta(this, & preconditioning::_delta),
            cut( numeric<T>::tiny ),
            one(1),
            zero(0),
            ctol(numeric<T>::ftol),
            xtol(numeric<T>::sqrt_ftol)
            {
            }

            //! cleanup
            inline virtual ~preconditioning() throw()
            {
            }


            //! find diagonal weights
            bool run(addressable<T>         &weight,
                     const matrix<T>        &curvature,
                     const accessible<bool> *used)
            {
                assert(!used||(used->size()==weight.size()));
                assert(weight.size()==curvature.rows);
                assert(weight.size()==curvature.cols);

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                array_type                             &cdiag = aliasing::_(curvature.r_aux1);
                array_type                             &fwksp = aliasing::_(curvature.r_aux2);
                const core::temporary_value<size_t>     nlink(nvar,weight.size());

                //--------------------------------------------------------------
                //
                // capture active diagonal components
                //
                //--------------------------------------------------------------
                const  bool full = (NULL==used);
                size_t      nrun = 0;
                for(size_t i=1;i<=nvar;++i)
                {
                    weight[i] = zero;
                    if(full || (*used)[i])
                    {
                        ++nrun;
                        cdiag[nrun] = curvature[i][i];
                    }
                }

                switch(nrun)
                {

                    case  1: set_active(weight,used); /* FALLTHRU */
                    case  0: return true;             // early return
                    default: break;
                }

                //--------------------------------------------------------------
                //
                // find dmin and dmax using adapted diag[1..nrun]
                //
                //--------------------------------------------------------------
                array_type                     diag(*cdiag,nrun); sort(diag);
                const core::temporary_value<T> kdmin(dmin,diag[1]);
                const core::temporary_value<T> kdmax(dmax,diag[nrun]);

                //--------------------------------------------------------------
                //
                // early returns
                //
                //--------------------------------------------------------------
                std::cerr << "curv=" << curvature << std::endl;
                std::cerr << "diag=" << diag      << std::endl;
                std::cerr << "dmin=" << dmin      << std::endl;
                std::cerr << "dmax=" << dmax      << std::endl;
                std::cerr << "q0  =" << dmin/dmax << std::endl;
                std::cerr << "ctol=" << ctol << std::endl;

                if(dmin<=0)
                {
                    return false; // singular curvature
                }



                //--------------------------------------------------------------
                //
                // look up
                //
                //--------------------------------------------------------------
                const core::temporary_link<array_type>  dlink(diag,&diag_);
                const core::temporary_value<T>          kdrho(drho,_drho());


                //--------------------------------------------------------------
                //
                // prepare a workspace to compute new values
                //
                //--------------------------------------------------------------
                array_type                              wksp(*fwksp,nrun);
                const core::temporary_link<array_type>  wlink(wksp,&wksp_);

#if 0
                {
                    ios::ocstream fp("omega.dat");
                    for(T p=dmax;p<=3*dmax;p+=0.001)
                    {
                        const T d = delta(p);
                        fp("%.20g %.20g %.20g\n",p/dmax,d,p);
                    }
                }
#endif

                //--------------------------------------------------------------
                //
                // built-in bracket/minimize
                //
                //--------------------------------------------------------------
                triplet<T> x     = { dmax,       -1, dmax+dmax  };
                triplet<T> f     = { delta(x.a), -1, delta(x.c) };
                T          width = fabs_of(x.c-x.a);
                while(true)
                {
                    static const unsigned B_GT_A  = 0x01;
                    static const unsigned B_GT_C  = 0x02;
                    assert(x.a<=x.c);
                    assert(width>=0);
                    f.b = ( delta( x.b = 0.5*(x.a+x.c) ) );
                    if( f.b <= ctol )
                    {
                        break; // numerical convergence
                    }
                    unsigned    flag  = 0x00;
                    if(f.b>f.a) flag |= B_GT_A;
                    if(f.b>f.c) flag |= B_GT_C;
                    switch(flag)
                    {
                        case B_GT_A:
                            // f.b>f.a, f.b<=f.c
                            // move c to b
                            x.c = x.b;
                            f.c = f.b;
                            break;

                        case B_GT_C:
                            // f.c > f.c, f.b<=f.c
                            // move a to b
                            x.a = x.b;
                            f.a = f.b;
                            break;

                        case B_GT_A|B_GT_C:
                            // got to make a choice..
                            if(f.a<=f.c)
                            {
                                x.c = x.b;
                                f.c = f.b;
                            }
                            else
                            {
                                x.a=x.b;
                                f.a=f.b;
                            }
                            break;

                        default:
                            // bracketed
                            break;
                    }
                    const T new_width = fabs_of(x.c-x.a);
                    const T max_width = fabs_of(xtol * x.b);
                    if(new_width>=width || width <= max_width )
                    {
                        break;
                    }
                    width = new_width;
                }

                const T    p    = x.b;
                const T    beta = cut+ max_of(p-drho,zero);
                std::cerr << "p=" << p << std::endl;


                for(size_t i=nvar;i>0;--i)
                {
                    if(full || (*used)[i])
                    {
                        weight[i] = max_of(p-curvature[i][i],zero)/beta;
                    }
                }

                (void) delta(p);
                const T q = wksp[1]/wksp[nrun];
                std::cerr << "q=" << q << " / " << dmin/dmax << std::endl;
                
                return true;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(preconditioning);
            size_t          nvar;
            T               dmin;
            T               dmax;
            T               drho;
            array_type     *diag_;
            array_type     *wksp_;
            function1d      delta;
            const T         cut,one,zero,ctol,xtol;

            inline T _drho() throw()
            {
                assert(diag_);
                const array_type &diag = *diag_;
                const size_t      nrun = diag.size();
                T sum=0, sum2=0;
                for(size_t i=1;i<=nrun;++i)
                {
                    const T d = diag[i];
                    sum  += d;
                    sum2 += square_of(d);
                }
                return min_of(sum2/sum,dmax);
            }



            inline T _delta(const T p) throw()
            {

                assert(diag_);
                assert(wksp_);
                assert(diag_->size()==wksp_->size());


                const T           beta = cut+max_of(p-drho,zero);
                array_type       &wksp = *wksp_;
                const array_type &diag = *diag_;
                const size_t      nrun = wksp.size();

                for(size_t j=nrun;j>0;--j)
                {
                    const T dj = diag[j];
                    const T wj = max_of(p-dj,zero)/beta;
                    wksp[j]    = dj * wj;
                }
                sort(wksp);
                return fabs_of(one-wksp[1]/wksp[nrun]);
            }

            inline void set_active(addressable<T>         &w,
                                   const accessible<bool> *used ) throw()
            {
                if(used)
                {
                    const accessible<bool> &u = *used;
                    assert(u.size()==w.size());
                    for(size_t i=w.size();i>0;--i)
                    {
                        if( u[i] )
                        {
                            w[i] = one;
                        }
                    }
                }
                else
                {
                    for(size_t i=w.size();i>0;--i)
                    {
                        w[i] = one;
                    }
                }
            }

            static inline void sort(addressable<T> &arr)
            {
                hsort(arr,comparison::increasing<T>);
            }

        };
    }
}

#endif
