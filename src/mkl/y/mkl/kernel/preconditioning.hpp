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

namespace upsylon
{
    namespace mkl
    {

        template <typename T>
        class preconditioning
        {
        public:
            typedef lightweight_array<T>   array_type; //!< alias
            typedef const accessible<bool> flags_type; //!< alias

            inline explicit preconditioning() :
            nvar(0),
            nrun(0),
            dmin(0),
            dmax(0),
            used_(0),
            diag_(0),
            temp_(0)
            {
            }

            inline virtual ~preconditioning() throw()
            {
            }

            bool run(addressable<T>         &weight,
                     const matrix<T>        &curv,
                     flags_type             &used)
            {
                assert(weight.size()==used.size());
                assert(weight.size()==curv.rows);
                assert(weight.size()==curv.cols);

                // initialize
                typename matrix<T>::row                &fdiag = aliasing::_(curv.r_aux1);
                const core::temporary_value<size_t>     nlink(nvar,weight.size());
                const core::temporary_link<flags_type>  ulink(used,&used_);

                // capture active diagonal components
                size_t active = 0;
                for(size_t i=nvar;i>0;--i)
                {
                    if(used[i])
                    {
                        ++active;
                        weight[i]      = 0;
                        fdiag[active] = fabs_of(curv[i][i]);
                    }
                    else
                    {
                        weight[i] = 1;
                    }
                }
                if(active<=0)
                {
                    return true;
                }

                // find dmin and dmax
                const core::temporary_value<size_t>    alink(nrun,active);
                array_type                             diag(*fdiag,nrun);
                const core::temporary_link<array_type> dlink(diag,&diag_);
                hsort(diag,comparison::increasing<T>);
                const core::temporary_value<T>         kdmin(dmin,diag[1]);
                const core::temporary_value<T>         kdmax(dmax,diag[nrun]);

                std::cerr << "curv=" << curv << std::endl;
                std::cerr << "diag=" << diag << std::endl;

                if(dmin<=0)
                {
                    // singular curvature
                    return false;
                }

                


                return true;


            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(preconditioning);
            size_t          nvar;
            size_t          nrun;
            T               dmin;
            T               dmax;
            flags_type     *used_;
            array_type     *diag_;
            array_type     *temp_;

            inline void find_dmin_and_dmax()
            {
                assert(dmax<=0);
                T vmax=(*diag_)[1],vmin=vmax;
                for(size_t i=nrun;i>1;--i)
                {
                    const T tmp = (*diag_)[i];
                    if(tmp>vmax)
                    {
                        vmax = tmp;
                    }
                    else if(tmp<vmin)
                    {
                        vmin = tmp;
                    }
                }
            }


        };
    }
}

#endif
