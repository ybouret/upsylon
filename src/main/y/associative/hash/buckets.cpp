

#include "y/associative/hash/buckets.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/self-destruct.hpp"
#include "y/object.hpp"

namespace upsylon
{

    hash_buckets:: ~hash_buckets() throw()
    {
        for(size_t i=0;i<buckets;++i)
        {
            self_destruct(bucket[i]);
        }
        object::dyadic_release(bucket,__bexp2);
        cleanup();
    }

    void hash_buckets:: cleanup() throw()
    {
        bucket = 0;
        aliasing::_(buckets) = 0;
        aliasing::_(__bmask) = 0;
        aliasing::_(__bexp2) = 0;
    }

    void hash_buckets:: release() throw()
    {
        for(size_t i=0;i<buckets;++i)
        {
            bucket[i].release();
        }
    }

    void hash_buckets:: swap_with(hash_buckets &other) throw()
    {
        _cswap(bucket,other.bucket);
        _cswap(buckets,other.buckets);
        _cswap(__bmask,other.__bmask);
        _cswap(__bexp2,other.__bexp2);
    }

    hash_buckets:: hash_buckets(const size_t n) :
    bucket(0),
    buckets( next_power_of_two<size_t>( clamp(min_size,n,max_size) ) ),
    __bmask(buckets-1),
    __bexp2( base2<size_t>::log2_of(buckets*sizeof(hash_bucket) ) )
    {
        // acquire memory
        try
        {
            bucket = static_cast<hash_bucket *>( object::dyadic_acquire(__bexp2) );
        }
        catch(...)
        {
            cleanup();
            throw;
        }

        // finalize
        for(size_t i=0;i<buckets;++i) new (bucket+i) hash_bucket();
    }

    hash_bucket & hash_buckets:: operator[](const size_t hkey) throw()
    {
        return bucket[hkey&__bmask];
    }

    const hash_bucket & hash_buckets:: operator[](const size_t hkey) const throw()
    {
        return bucket[hkey&__bmask];
    }


    void hash_buckets:: to(hash_bucket &pool) throw()
    {
        for(size_t i=0;i<buckets;++i)
        {
            pool.merge_back(bucket[i]);
        }
    }

    void hash_buckets:: to(hash_buckets &other) throw()
    {
        for(size_t i=0;i<buckets;++i)
        {
            hash_bucket &b = bucket[i];
            while(b.size)
            {
                other.insert( b.pop_back() );
            }
        }
    }

    void  hash_buckets:: insert( hash_handle *handle ) throw()
    {
        assert(handle);
        (*this)[handle->hkey].push_front(handle);
    }




}


#include <iostream>
#include <iomanip>

namespace upsylon
{
    void hash_buckets:: dump() const
    {
        std::cerr << std::hex;
        std::cerr << "<hash_buckets>" << std::endl;
        for(size_t i=0;i<buckets;++i)
        {
            std::cerr << "[" << std::setw(4) << i << "] :";
            for(const hash_handle *node=bucket[i].head;node;node=node->next)
            {
                std::cerr << " " << node->hkey;
            }
            std::cerr << std::endl;
        }

        std::cerr << "<hash_buckets/>" << std::endl;
        std::cerr << std::dec;
    }
}
