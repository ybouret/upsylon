

//! \file

#ifndef Y_HASH_BUCKETS_INCLUDED
#define Y_HASH_BUCKETS_INCLUDED 1

#include "y/associative/hash/bucket.hpp"
#include "y/code/base2.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! collection of buckets
    //
    //__________________________________________________________________________
    class hash_buckets
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        static const size_t required_min_size = 4; //!< minimal bucket count
        static const size_t max_size          = base2<size_t>::max_power_of_two / sizeof(hash_bucket);       //!< maximal buckets
        static const size_t min_size          = max_size < required_min_size ? max_size : required_min_size; //!< minimal buckets

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        hash_buckets(const size_t n); //!< setup next_power_of_two(clamp(min_size,n,max_size)) buckets
        ~hash_buckets() throw();      //!< release all memory

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        void swap_with(hash_buckets &other) throw();                     //!< no-throw swap
        void release() throw();                                          //!< release handles

        hash_bucket       & operator[](const size_t hkey)       throw(); //!< access
        const hash_bucket & operator[](const size_t hkey) const throw(); //!< const access

        void to(hash_bucket  &pool)  throw(); //!< move all content to pool
        void to(hash_buckets &other) throw(); //!< redispacth nodes

        void insert(hash_meta *handle) throw(); //!< insert handle w.r.t its key

        void dump() const; //!< display buckets/keys

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
