
//! \file

#ifndef Y_MATH_ADJUST_SAMPLE_LOAD_INCLUDED
#define Y_MATH_ADJUST_SAMPLE_LOAD_INCLUDED 1

#include "y/math/adjust/sample.hpp"
#include "y/math/io/data-set.hpp"
#include "y/ios/icstream.hpp"

namespace upsylon {
    
    namespace math {
        
        namespace Adjust {

            //! fast loading and setup of samples
            struct Load
            {
                //! load from a stream
                template <typename T> static inline
                size_t From(ios::istream &fp,
                            Sample<T>    &sample,
                            const size_t  abscissa_index,
                            const size_t  ordinate_index,
                            const size_t skip=0,
                            const size_t nmax=0)
                {
                    sequence<T> &x = *aliasing::_(sample.abscissa);
                    sequence<T> &y = *aliasing::_(sample.ordinate);
                    sequence<T> &z = *sample.adjusted;
                    x.free();
                    y.free();
                    z.free();

                    {
                        data_set<T> ds;
                        ds.use(abscissa_index,x);
                        ds.use(ordinate_index,y);
                        ds.load(fp,skip,nmax);
                    }
                    z.adjust(x.size(),0);
                    
                    return sample.count();
                }

                //! load from a file
                template <typename T> static inline
                size_t From(const string &dataFile,
                            Sample<T>    &sample,
                            const size_t  abscissa_index,
                            const size_t  ordinate_index,
                            const size_t skip=0,
                            const size_t nmax=0)
                {
                    ios::icstream fp(dataFile);
                    return From(fp,sample,abscissa_index,ordinate_index,skip,nmax);
                }

                //! load from a file, wrapper
                template <typename T> static inline
                size_t From(const char   *dataFile,
                            Sample<T>    &sample,
                            const size_t  abscissa_index,
                            const size_t  ordinate_index,
                            const size_t skip=0,
                            const size_t nmax=0)
                {
                    const string _(dataFile);
                    return From(_,sample,abscissa_index,ordinate_index,skip,nmax);
                }
            };
            
        }
    }
}

#endif

