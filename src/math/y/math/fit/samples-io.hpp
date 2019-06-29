//! \file
#ifndef Y_FIT_SAMPLES_IO_INCLUDED
#define Y_FIT_SAMPLES_IO_INCLUDED 1

#include "y/math/io/data-set.hpp"
#include "y/math/fit/vectors.hpp"
#include "y/ios/ocstream.hpp"

namespace upsylon
{
    namespace math
    {

        namespace Fit
        {

            //! wrappers to simplify I/O of samples
            struct IO
            {

                //! save X/Y/Yfit
                template <typename T> static inline
                void Save( ios::ostream &fp, const Sample<T> &sample )
                {
                    for(size_t i=1;i<=sample.count();++i)
                    {
                        fp("%.15g %.15g %.15g\n", double(sample.X[i]), double(sample.Y[i]), double(sample.Yf[i]));
                    }
                }

                //! save wrapper
                template <typename T> static inline
                void Save( const string &filename, const Sample<T> &sample )
                {
                    ios::ocstream fp(filename); Save(fp,sample);
                }

                //! save wrapper
                template <typename T> static inline
                void Save( const char *filename, const Sample<T> &sample )
                {
                    const string _(filename); Save(_,sample);
                }


                //! load X/Y and prepare Yf
                template <typename T> static inline
                size_t Load(const string    &filename,
                            const size_t    ix,
                            sequence<T>     &X,
                            const size_t    iy,
                            sequence<T>     &Y,
                            sequence<T>     &Yf,
                            const size_t    skip=0,
                            const size_t    nmax=0)
                {
                    Yf.free();
                    const size_t n = data_set<T>::loadXY(filename, ix, X, iy, Y,skip,nmax);
                    Yf.ensure(n);
                    for(size_t i=n;i>0;--i) Yf.push_back(0);
                    return n;
                }

                //! wrappper
                template <typename T> static inline
                size_t Load(const char *    filename,
                            const size_t    ix,
                            sequence<T>     &X,
                            const size_t    iy,
                            sequence<T>     &Y,
                            sequence<T>     &Yf,
                            const size_t    skip=0,
                            const size_t    nmax=0)
                {
                    const string _(filename);
                    return Load<T>(_,ix,X,iy,Y,Yf,skip,nmax);
                }
                
                //! load a set of vectors
                template <typename T> static inline
                size_t Load(const string   &filename,
                            Vectors<T>     &vecs,
                            const size_t    ix,
                            const size_t    iy,
                            const size_t    skip=0,
                            const size_t    nmax=0)
                {
                    return Load<T>(filename,ix,vecs.X,iy,vecs.Y,vecs.Yf,skip,nmax);
                }

                //! load a set of vectors, wrapper
                template <typename T> static inline
                size_t Load(const char     *filename,
                            Vectors<T>     &vecs,
                            const size_t    ix,
                            const size_t    iy,
                            const size_t    skip=0,
                            const size_t    nmax=0)
                {
                    const string _(filename);
                    return Load<T>(_,vecs,ix,iy,skip,nmax);
                }
            };

        }

    }
}

#endif

