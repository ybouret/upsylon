
//! \file

#ifndef Y_FITTING_SAMPLE_LOAD_INCLUDED
#define Y_FITTING_SAMPLE_LOAD_INCLUDED 1

#include "y/mkl/fitting/sample.hpp"
#include "y/mkl/io/data-set.hpp"
#include "y/ios/icstream.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! helper to load sample
            //
            //__________________________________________________________________
            struct load_sample
            {
                //______________________________________________________________
                //
                //! genereric call
                //______________________________________________________________
                template <typename T> static inline
                size_t from(ios::istream &fp,
                            sample<T,T>  &s,
                            const size_t iA,
                            const size_t iO,
                            const size_t skip=0,
                            const size_t nmax=0)
                {

                    data_set<T> ds;
                    ds.use(iA,*s.abscissa);
                    ds.use(iO,*s.ordinate);
                    try
                    {
                        ds.load(fp,skip,nmax);
                        s.validate();
                        return s.count();
                    }
                    catch(...)
                    {
                        s.free();
                        throw;
                    }
                }
                
                //______________________________________________________________
                //
                //! from file
                //______________________________________________________________
                template <typename ID, typename T> static inline
                size_t from_file(const ID     &id,
                                 sample<T,T>  &s,
                                 const size_t iA,
                                 const size_t iO,
                                 const size_t skip=0,
                                 const size_t nmax=0)
                {
                    ios::icstream fp(id);
                    return from(fp,s,iA,iO,skip,nmax);
                }

            };

        }

    }

}

#endif
