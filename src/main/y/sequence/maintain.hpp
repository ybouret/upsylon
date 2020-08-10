//! \file
#ifndef Y_SEQUENCE_MAINTAIN_INCLUDED
#define Y_SEQUENCE_MAINTAIN_INCLUDED 1

#include "y/sequence/array.hpp"
#include "y/sequence/list.hpp"
#include "y/type/block/move.hpp"
#include "y/memory/zblock.hpp"
#include "y/memory/allocator/global.hpp"

namespace upsylon
{
    //! function to maintain a sequence of good values
    struct maintain
    {
        //! build a list of indices in [1..n] to keep, if keep(it) = flag
        template <
        typename ITERATOR,
        typename KEEP_PROC> static inline
        void build_indices(sequence<size_t> &indices,
                           ITERATOR          it,
                           const size_t      n,
                           KEEP_PROC        &keep,
                           const bool        flag )
        {
            indices.free();
            indices.ensure(n);
            for(size_t i=1;i<=n;++i,++it)
            {
                if( keep(*it) == flag )
                {
                    indices.push_back(i);
                }
            }
        }

        //! wrapper for sequences of data
        template <
        typename SEQUENCE,
        typename KEEP_PROC> static inline
        void build_indices(sequence<size_t> &indices,
                           const SEQUENCE   &data,
                           KEEP_PROC        &keep,
                           const bool        flag)
        {
            build_indices(indices,data.begin(),data.size(),keep,flag);
        }

        //! rearrange array to keep (in same order) valid indices first
        template <
        typename T,
        typename INDEX_ITERATOR
        > static inline
        void rearrange( array<T> &arr, INDEX_ITERATOR ii, const size_t ni )
        {
            const size_t           na = arr.size();
            size_t                 ia = 1;

            zblock<T,memory::global>  blk(na);
            size_t       ib = 1;
            size_t       nb = na;
            for(size_t i=ni;i>0;--i,++ii)
            {
                const size_t ja = *ii; assert(ja<=na);
                // put invalid indices at the end
                while(ia<ja)
                {
                    bmove(blk[nb--],arr[ia++]);
                }
                assert(ia==ja);
                // push back good index
                bmove(blk[ib++],arr[ia++]);
            }
            while(ia<=na)
            {
                // put remaining invalid indices at the end
                bmove(blk[nb--],arr[ia++]);
            }
            assert(nb==ni);
            // put back into place
            for(size_t i=na;i>0;--i)
            {
                bmove(arr[i],blk[i]);
            }
        }

        //! rearrange list to keep (in same order) valid indices first
        template <
        typename T,
        typename INDEX_ITERATOR
        > static inline
        void rearrange( list<T> &L, INDEX_ITERATOR ii, const size_t ni )
        {
#if !defined(NDEBUG)
            const size_t na = L.size();
#endif
            typename list<T>::nodes_list source,good,bad;
            L.__send_all(source); assert(source.size==na);
            size_t                 ia = 1;
            for(size_t i=ni;i>0;--i,++ii)
            {
                assert(ia+source.size==na+1);
                // next good index
                const size_t ja = *ii; assert(ja<=na);
                while(ia<ja)
                {
                    // put invalid at the end
                    bad.push_back( source.pop_front() );
                    ++ia;
                }
                // put good index into place
                good.push_back( source.pop_front() );
                ++ia;
            }
            // reconstruct list
            L.__recv_all(good);
            L.__recv_all(bad);
            L.__recv_all(source);
        }


        //! pop_back invalid items after rearrange
        template <
        typename DATA_SEQ,
        typename INDX_SEQ> static inline
        void keep( DATA_SEQ &data, INDX_SEQ &indx )
        {
            assert(indx.size()<=data.size());
            rearrange(data, indx.begin(), indx.size() );
            const size_t ngood = indx.size();
            while(data.size()>ngood)
            {
                data.pop_back();
            }
        }

    };

}

#endif

