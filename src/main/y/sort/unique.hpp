//! \file
#ifndef Y_UNIQUE_INCLUDED
#define Y_UNIQUE_INCLUDED 1

#include "y/comparison.hpp"
#include "y/sort/heap.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/array.hpp"
#include "y/type/memops.hpp"

namespace upsylon {

    namespace core {

        //! sort an array and put multiple values at the end
        template <typename T>
        size_t __unique( array<T> &arr )
        {
            hsort(arr,comparison::increasing<T>);
            size_t n = arr.size();
            size_t i = 1;
            while(i<n)
            {
                const size_t j   = i+1;
                const int    cmp = comparison::increasing(arr[i],arr[j]);
                assert(cmp<=0);
                if(0==cmp)
                {
                    // move j at the end
                    for(size_t k=j,kp=k+1;k<n;++k,++kp)
                    {
                        upsylon::memops::swap(arr[k],arr[kp]);
                    }
                    --n;
                }
                else
                {
                    assert(cmp<0);
                    i=j;
                }
            }
            return n;
        }

        //! sort a list and put multiple values at the end
        template <typename T>
        size_t __unique( list<T> &L )
        {
            L.sort(comparison::increasing<T>);
            typename list<T>::nodes_list nodes;
            L.__send_all(nodes);
            typename list<T>::nodes_list single,multiple;
            while(nodes.size)
            {
                typename list<T>::node_type *node = nodes.pop_front();
                if(single.size<=0)
                {
                    single.push_back(node);
                }
                else
                {
                    const int cmp = comparison::increasing(single.tail->data,node->data);
                    assert(cmp<=0);
                    if(0==cmp)
                    {
                        multiple.push_back(node);
                    }
                    else
                    {
                        assert(cmp<0);
                        single.push_back(node);
                    }
                }
            }
            const size_t ans = single.size;
            L.__recv_all(single);
            L.__recv_all(multiple);
            return ans;
        }
    }

    //! sort and keep unique value
    template <typename SEQUENCE>
    void unique( SEQUENCE &seq )
    {
        const size_t new_size = core::__unique(seq);
        while(seq.size()>new_size)
        {
            seq.pop_back();
        }
    }




}

#endif

