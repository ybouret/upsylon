
//! \file
#ifndef Y_MKL_UTILS_INCLUDED
#define Y_MKL_UTILS_INCLUDED 1

#include "y/mkl/types.hpp"
#include "y/container/matrix.hpp"

namespace upsylon {

    namespace mkl {

        //! simplification for sets of integer types
        template <typename T>
        struct simplify
        {
            Y_DECL_ARGS(T,type);

            static inline
            type on( addressable<type> &data )
            {
                type         r  = 1;
                const size_t n  = data.size();
                type         lo = 0; // lowest not nul absolute value
                type         hi = 0; // highest not nul absolute value
                {
                    size_t i=n;
                    for(;i>0;--i)
                    {
                        const_type a = abs_of(data[i]);
                        if(a>0)
                        {
                            lo = a;
                            break;
                        }
                    }
                    hi = lo;
                    for(--i;i>0;--i)
                    {
                        const type a = abs_of(data[i]);
                        if(a>0)
                        {
                            if(a<lo)
                            {
                                lo = a;
                            }
                            else
                            {
                                if(a>hi)
                                {
                                    hi = a;
                                }
                            }

                        }
                    }
                }
                if(hi>0)
                {
                    for(type d=lo;d>1;--d)
                    {
                        bool ok = true;
                        for(size_t i=n;i>0;--i)
                        {
                            const_type &a = data[i];
                            if(a!=0&&0!=(a%d))
                            {
                                ok=false;
                                break;
                            }
                        }
                        if(ok)
                        {
                            r=d;
                            break;
                        }
                    }
                    for(size_t i=n;i>0;--i)
                    {
                        data[i] /= r;
                    }
                }
                return r;
            }

            static inline
            void on( addressable<type> &r, matrix<type> &M )
            {
                assert( r.size() >= M.rows );
                for(size_t i=M.rows;i>0;--i)
                {
                    r[i] = on(M[i]);
                }
            }

            static inline
            type on( matrix<type> &M )
            {
                typename matrix<type>::row data = M.as_array();
                return on(data);
            }

            static inline
            type on( addressable<T> &data, T &level )
            {
                type       r   = 1;
                const_type ref = abs_of(level);
                type       lo  = ref;
                if(lo>0)
                {
                    const size_t n  = data.size();
                    type         hi = lo;
                    for(size_t i=n;i>0;--i)
                    {
                        const_type a = abs_of(data[i]);
                        if(a>0)
                        {
                            if(a<lo)
                            {
                                lo = a;
                            }
                            else
                            {
                                if(a>hi)
                                {
                                    hi = a;
                                }
                            }
                        }
                    }

                    if(hi>0)
                    {
                        const_type ref = abs_of(level);
                        for(type d=lo;d>1;--d)
                        {
                            if(0!=(ref%d)) continue; // ref is not divided
                            bool ok = true;
                            for(size_t i=n;i>0;--i)
                            {
                                const_type &a = data[i];
                                if(a!=0&&0!=(a%d))
                                {
                                    ok=false;
                                    break;
                                }
                            }
                            if(ok)
                            {
                                r=d;
                                break;
                            }
                        }

                        if(r>1)
                        {
                            level/=r;
                            for(size_t i=n;i>0;--i)
                            {
                                data[i] /= r;
                            }
                        }

                    }
                }
                return r;
            }

            static inline
            type on( matrix<T> &M, T &level )
            {
                typename matrix<T>::row data = M.as_array();
                return on(data,level);
            }

        };

    }

}

#endif
