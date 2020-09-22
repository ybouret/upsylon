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
#include "y/mkl/opt/bracket.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace upsylon
{
    namespace mkl
    {

        template <typename T>
        class preconditioning
        {
        public:
            typedef lightweight_array<T>            array_type; //!< alias
            typedef const accessible<bool>          flags_type; //!< alias
            typedef typename numeric<T>::function   function1d; //!< alias

            inline explicit preconditioning() :
            nvar(0),
            dmin(0),
            dmax(0),
            del0(0),
            used_(0),
            diag_(0),
            wksp_(0),
            delta(this, & preconditioning::_delta)
            {
            }

            inline virtual ~preconditioning() throw()
            {
            }

            bool run(addressable<T>         &weight,
                     const matrix<T>        &curvature,
                     flags_type             &used)
            {
                assert(weight.size()==used.size());
                assert(weight.size()==curvature.rows);
                assert(weight.size()==curvature.cols);

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                array_type                             &fdiag = aliasing::_(curvature.r_aux1);
                array_type                             &fwksp = aliasing::_(curvature.r_aux2);
                const core::temporary_value<size_t>     nlink(nvar,weight.size());
                const core::temporary_link<flags_type>  ulink(used,&used_);

                //--------------------------------------------------------------
                //
                // capture active diagonal components
                //
                //--------------------------------------------------------------
                size_t nrun = 0;
                for(size_t i=1;i<=nvar;++i)
                {
                    weight[i] = 0;
                    if(used[i])
                    {
                        ++nrun;
                        fdiag[nrun] = curvature[i][i];
                    }

                }

                if(nrun<=0)
                {
                    return true;
                }

                //--------------------------------------------------------------
                //
                // find dmin and dmax using adapted diag[1..nrun]
                //
                //--------------------------------------------------------------
                array_type                              diag(*fdiag,nrun);
                const core::temporary_link<array_type>  dlink(diag,&diag_);

                hsort(diag,comparison::increasing<T>);
                const core::temporary_value<T> kdmin(dmin,diag[1]);
                const core::temporary_value<T> kdmax(dmax,diag[nrun]);
                const core::temporary_value<T> kdel0(del0,dmax-dmin);

                std::cerr << "curv=" << curvature << std::endl;
                std::cerr << "diag=" << diag << std::endl;
                std::cerr << "dmin=" << dmin << std::endl;
                std::cerr << "dmax=" << dmax << std::endl;
                std::cerr << "del0=" << del0 << std::endl;


                if(dmin<=0)
                {
                    // singular curvature
                    return false;
                }

                std::cerr << "inv_cond0=" << dmax/dmin << std::endl;

                const T s2min = numeric<T>::epsilon * del0 / dmax;
                std::cerr << "s2min=" << s2min << std::endl;

                //--------------------------------------------------------------
                //
                // prepare workspace
                //
                //--------------------------------------------------------------
                array_type                              wksp(*fwksp,nrun);
                const core::temporary_link<array_type>  wlink(wksp,&wksp_);

                {
                    ios::ocstream fp("precond.dat");
                    const T s2max = 1.0;
                    const T l2min = log(s2min);
                    const T l2max = log(s2max);
                    const T l2amp = l2max - l2min;
                    for(int i=0;i<=1000;++i)
                    {
                        const T l2 = l2min + (i*l2amp)/1000;
                        const T s2 = exp(l2);
                        const T d2 = delta(s2);
                        fp("%.15g %.15g %.15g\n",s2,d2,log(_omega0(s2)));
                    }
                }

                //--------------------------------------------------------------
                //
                // find optimal distribution
                //
                //--------------------------------------------------------------

                triplet<T> S2 = { s2min,       -1, 1           };
                triplet<T> D2 = { delta(S2.a), -1, delta(S2.c) };
                bracket::inside(delta, S2, D2);
                const T    s2 = minimize::run(delta, S2, D2);
                std::cerr << "s2=" << s2 << std::endl;

                for(size_t i=1;i<=nvar;++i)
                {
                    if(used[i])
                    {
                        weight[i] = _omega(s2,curvature[i][i]);
                    }
                }
                std::cerr << "weight=" << weight << std::endl;

                delta(s2);
                std::cerr << "inv_cond=" << wksp[nrun]/wksp[1] << std::endl;


                return true;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(preconditioning);
            size_t          nvar;
            T               dmin;
            T               dmax;
            T               del0;
            flags_type     *used_;
            array_type     *diag_;
            array_type     *wksp_;
            function1d      delta;

            inline T _omega(const double s2, const double dj) throw()
            {
                static const T one = T(1);
                static const T cut = numeric<T>::tiny;

                assert(dj>=dmin);
                assert(dj<=dmax);
                const double del = clamp<T>(0,dj-dmin,del0);
                return one - del/(cut+del0+s2*dmax);
            }

            inline T _omega0(const double s2) throw()
            {
                static const T one = T(1);
                static const T cut = numeric<T>::tiny;
                return one - del0/(cut+del0+s2*dmax);
            }


            inline T _delta(const T s2) throw()
            {
                assert(used_); assert(used_->size()==nvar);
                assert(diag_);
                assert(wksp_);
                assert(diag_->size()==wksp_->size());

                //static const T    one  = T(1);
                //const T           fac  = one + s2;
                array_type       &wksp = *wksp_;
                const array_type &diag = *diag_;
                const size_t      nrun = wksp.size();

                for(size_t j=nrun;j>0;--j)
                {
                    const T dj = diag[j];
                    const T wj = _omega(s2,dj);
                    wksp[j]    = dj * wj;

                }
                hsort(wksp,comparison::increasing<T>);
                return fabs_of(wksp[nrun]-wksp[1]);
            }


        };
    }
}

#endif
