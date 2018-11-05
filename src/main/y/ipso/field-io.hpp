//! \file
#ifndef Y_IPSO_FIELD_IO_INCLUDED
#define Y_IPSO_FIELD_IO_INCLUDED 1

#include "y/ipso/layout.hpp"
#include "y/container/sequence.hpp"
#include "y/sort/unique.hpp"
#include "y/ipso/field.hpp"

namespace upsylon
{
    namespace ipso
    {
        struct field_io
        {
            //! collect indices from a sub layout
            template <typename LAYOUT>
            static void __collect( sequence<coord1D> &indices, const LAYOUT &full, const LAYOUT &sub );

            //! collect1D
            template <typename SEQUENCE>
            static inline void collect( SEQUENCE &indices, const layout1D &full, const layout1D &sub )
            {
                __collect<layout1D>(indices,full,sub);
                unique(indices);
            }

            //! collect2D
            template <typename SEQUENCE>
            static inline void collect( SEQUENCE &indices, const layout2D &full, const layout2D &sub )
            {
                __collect<layout2D>(indices,full,sub);
                unique(indices);
            }

            //! collect3D
            template <typename SEQUENCE>
            static inline void collect( SEQUENCE &indices, const layout3D &full, const layout3D &sub )
            {
                __collect<layout3D>(indices,full,sub);
                unique(indices);
            }


            //! query n items , target bytes >= n*sizeof(T)
            template <typename ITERATOR,typename T>
            static inline void query(uint8_t *      &target,
                                     const field<T> &source,
                                     ITERATOR        it,
                                     size_t          n)
            {
                assert(target);
                assert(source.entry);
                const T *q = source.entry;
                while(n-->0)
                {
                    const coord1D idx = *it;++it;
                    assert(source.is_acceptable(idx));
                    memcpy(target,&q[idx],sizeof(T));
                    target += sizeof(T);
                }
            }

            //! query items from sequence of indices
            template <typename SEQUENCE,typename T>
            static inline void query(uint8_t *      &target,
                                     const field<T> &source,
                                     const SEQUENCE &seq)
            {
                query(target,source,seq.begin(),seq.size());
            }

            template <typename ITERATOR,typename T>
            static inline void store(field<T>        &target,
                                     const uint8_t * &source,
                                     ITERATOR         it,
                                     size_t           n)
            {
                assert(source);
                assert(target.entry);
                T *p = target.entry;
                while(n-->0)
                {
                    const coord1D idx = *it;++it;
                    assert(target.is_acceptable(idx));
                    memcpy(&p[idx],source,sizeof(T));
                    source += sizeof(T);
                }
            }

            template <typename SEQUENCE,typename T>
            static inline void store(field<T>        &target,
                                     const uint8_t * &source,
                                     const SEQUENCE  &seq )
            {
                store(target,source,seq.begin(),seq.size());
            }

            
        };
    }
}

#endif

