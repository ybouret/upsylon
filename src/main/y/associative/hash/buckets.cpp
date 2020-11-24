

#include "y/associative/hash/buckets.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/self-destruct.hpp"
#include "y/object.hpp"

namespace upsylon
{

    hash_buckets:: ~hash_buckets() throw()
    {
        for(size_t i=0;i<count;++i)
        {
            self_destruct(bucket[i]);
        }
        object::dyadic_release(bucket,bexp2);
        cleanup();
    }

    void hash_buckets:: cleanup() throw()
    {
        bucket = 0;
        aliasing::_(count) = 0;
        aliasing::_(bmask) = 0;
        aliasing::_(bexp2) = 0;
    }

    void hash_buckets:: release() throw()
    {
        for(size_t i=0;i<count;++i)
        {
            bucket[i].release();
        }
    }

    void hash_buckets:: swap_with(hash_buckets &other) throw()
    {
        _cswap(bucket,other.bucket);
        _cswap(count,other.count);
        _cswap(bmask,other.bmask);
        _cswap(bexp2,other.bexp2);
    }

    static inline size_t buckets_for(const size_t n) throw()
    {
        return next_power_of_two<size_t>( clamp(hash_buckets::min_size,n,hash_buckets::max_size) );
    }

    hash_buckets:: hash_buckets(const size_t n) :
    bucket(0),
    count( buckets_for(n) ),
    bmask(count-1),
    bexp2( base2<size_t>::log2_of(count*sizeof(hash_bucket) ) )
    {
        // acquire memory
        try
        {
            bucket = static_cast<hash_bucket *>( object::dyadic_acquire(bexp2) );
        }
        catch(...)
        {
            cleanup();
            throw;
        }

        // finalize
        for(size_t i=0;i<count;++i) new (bucket+i) hash_bucket();
    }

    hash_bucket & hash_buckets:: operator[](const size_t hkey) throw()
    {
        return bucket[hkey&bmask];
    }

    const hash_bucket & hash_buckets:: operator[](const size_t hkey) const throw()
    {
        return bucket[hkey&bmask];
    }


    void hash_buckets:: to(hash_bucket &pool) throw()
    {
        for(size_t i=0;i<count;++i)
        {
            pool.merge_back(bucket[i]);
        }
    }

    void hash_buckets:: to(hash_buckets &other) throw()
    {
        for(size_t i=0;i<count;++i)
        {
            hash_bucket &b = bucket[i];
            while(b.size)
            {
                other.insert( b.pop_back() );
            }
        }
    }

    void  hash_buckets:: insert(hash_meta *handle) throw()
    {
        assert(handle);
        (*this)[handle->hkey].push_front(handle);
    }

    hash_meta * hash_buckets:: remove(hash_meta *meta) throw()
    {
        assert(meta);
        return (*this)[meta->hkey].unlink(meta);
    }

    size_t hash_buckets:: for_load_factor(const size_t load_factor, const size_t entries) throw()
    {
        if( load_factor <= 1 )
        {
            return buckets_for(entries);
        }
        else
        {
            size_t n = entries/load_factor;
            while(n*load_factor<entries) ++n;
            return buckets_for(n);
        }
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
        for(size_t i=0;i<count;++i)
        {
            std::cerr << "[" << std::setw(4) << i << "] :";
            for(const hash_meta *node=bucket[i].head;node;node=node->next)
            {
                std::cerr << " " << node->hkey;
            }
            std::cerr << std::endl;
        }

        std::cerr << "<hash_buckets/>" << std::endl;
        std::cerr << std::dec;
    }
}
