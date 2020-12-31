//! \file

#ifndef Y_FITTING_SEQUENTIAL_INCLUDED
#define Y_FITTING_SEQUENTIAL_INCLUDED 1

#include "y/mkl/fitting/variables.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! sequential calls interface
            //
            //__________________________________________________________________
            template <typename T,
            typename ABSCISSA,
            typename ORDINATE>
            class sequential
            {
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
            public:
                //! cleanup
                inline virtual ~sequential() throw() { _bzset(current); }

            protected:
                //! setup
                inline explicit sequential() throw() : current(0) {}

            public:
                //______________________________________________________________
                //
                // non virtual interface
                //______________________________________________________________

                //! make first call
                inline ORDINATE start(const ABSCISSA x, const accessible<T> &aorg, const variables &vars)
                {
                    const ORDINATE   ans = onStart(x,aorg,vars);
                    aliasing::_(current) = x;
                    return ans;
                }

                //! make subsequent calls
                inline ORDINATE reach(const ABSCISSA x, const accessible<T> &aorg, const variables &vars)
                {
                    const ORDINATE   ans = onReach(x,aorg,vars);
                    aliasing::_(current) = x;
                    return ans;
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const ABSCISSA current; //!< current abscissa

            private:
                Y_DISABLE_COPY_AND_ASSIGN(sequential);
                virtual ORDINATE onStart(const ABSCISSA, const accessible<T> &,const variables &) = 0;
                virtual ORDINATE onReach(const ABSCISSA, const accessible<T> &,const variables &) = 0;
            };
        }

    }

}

#endif
