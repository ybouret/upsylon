

//! \file

#ifndef Y_HASH_BUCKETS_INCLUDED
#define Y_HASH_BUCKETS_INCLUDED 1

#include "y/associative/hash/bucket.hpp"
#include "y/code/base2.hpp"

namespace upsylon
{

    class hash_buckets
    {
    public:
        static const size_t required_min_size = 4;
        static const size_t max_size          = base2<size_t>::max_power_of_two / sizeof(hash_bucket);
        static const size_t min_size          = max_size < required_min_size ? max_size : required_min_size;

        hash_buckets(const size_t n);
        ~hash_buckets() throw();

        void swap_with(hash_buckets &other) throw();
        void release() throw();

        hash_bucket       & operator[](const size_t hkey)       throw();
        const hash_bucket & operator[](const size_t hkey) const throw();

        void to(hash_bucket  &pool)  throw(); //!< move all content to pool
        void to(hash_buckets &other) throw(); //!< redispacth nodes

        void insert( hash_handle *handle ) throw(); //!< insert handle w.r.t its key

        void dump() const;

    private:
        hash_bucket *bucket;
    public:
        const size_t buckets;      //!< number of buckets, a power of two
        const size_t __bmask;      //!< mask for access = slots-1
        const size_t __bexp2;      //!< ilog2(slots*sizeof(hash_bucket))

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_buckets);
        void cleanup() throw();
    };


}

#endif
