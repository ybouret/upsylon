#include "y/information/bwt.hpp"
#include "y/sort/heap.hpp"
#include "y/sequence/array.hpp"

namespace upsylon {

    namespace information {

        namespace {

            struct rotlexdat
            {
                const uint8_t *buf;
                size_t         len;


                inline int operator()( size_t li, size_t ri ) const throw()
                {
                    if( li == ri )
                        return 0;

                    size_t ac = len;
                    while( buf[li] == buf[ri] )
                    {
                        li = (li+1) % len;
                        ri = (ri+1) % len;
                        if( --ac == 0 )
                            return 0;
                    }
                    return (buf[li] > buf[ri]) ? 1 : -1;
                }
            };
        }

        size_t bwt::encode( void *output, const void *input, const size_t size, size_t *indices) throw()
        {
            assert(!(NULL==output&&size>0));
            assert(!(NULL==input&&size>0));

            if(size<=0)
            {
                return 0;
            }
            else
            {
                const uint8_t    *buf_in  = (const uint8_t *)input;
                uint8_t          *buf_out = (uint8_t       *)output;

                for(size_t i=0;i<size;++i) indices[i] = i;
                lightweight_array<size_t>  arr( indices, size );

                rotlexdat         cmp = { buf_in, size };
                hsort( arr, cmp );

                size_t       pidx = 0;
                const size_t shft = size-1;
                size_t       ii   = 0;

                for(; ii < size; ++ii )
                {
                    const size_t idx = indices[ii];
                    buf_out[ii] = buf_in[ (idx+shft) % size];
                    if( 0 == idx )
                    {
                        pidx=ii;
                        break;
                    }
                }

                for(++ii;ii<size;++ii)
                {
                    buf_out[ii] = buf_in[ (indices[ii]+shft) % size];
                }

                return pidx;
            }
        }

        size_t bwt::encode(void *output, const void *input, const size_t size, size_t *indices, shaker &proc) throw()
        {
            assert(!(NULL==output&&size>0));
            assert(!(NULL==input&&size>0));

            if(size<=0)
            {
                return 0;
            }
            else
            {
                const uint8_t    *buf_in  = (const uint8_t *)input;
                uint8_t          *buf_out = (uint8_t       *)output;

                for(size_t i=0;i<size;++i) indices[i] = i;
                lightweight_array<size_t>  arr( indices, size );

                rotlexdat         cmp = { buf_in, size };
                hsort( arr, cmp );

                size_t       pidx = 0;
                const size_t shft = size-1;
                size_t       ii   = 0;

                for(; ii < size; ++ii )
                {
                    const size_t idx = indices[ii];
                    buf_out[ii] = proc.fetch(buf_in[ (idx+shft) % size]);
                    if( 0 == idx )
                    {
                        pidx=ii;
                        break;
                    }
                }

                for(++ii;ii<size;++ii)
                {
                    buf_out[ii] = proc.fetch(buf_in[ (indices[ii]+shft) % size]);
                }

                return pidx;
            }
        }



#define Y_BWT_Z8  0,0,0,0,0,0,0,0
#define Y_BWT_Z16  Y_BWT_Z8,   Y_BWT_Z8
#define Y_BWT_Z32  Y_BWT_Z16,  Y_BWT_Z16
#define Y_BWT_Z64  Y_BWT_Z32,  Y_BWT_Z32
#define Y_BWT_Z128 Y_BWT_Z64,  Y_BWT_Z64
#define Y_BWT_Z256 Y_BWT_Z128, Y_BWT_Z128


        void   bwt:: decode( void *output, const void *input, const size_t size, size_t *indices, const size_t primary_index) throw()
        {
            assert(!(NULL==output&&size>0));
            assert(!(NULL==input&&size>0));

            size_t         buckets[] = {Y_BWT_Z256};         assert(256==sizeof(buckets)/sizeof(buckets[0]));
            const uint8_t *buf_in    = (const uint8_t *)input;
            uint8_t       *buf_out   = (uint8_t       *)output;

            for( size_t i=0; i < size; ++i )
            {
                const size_t bi = buf_in[i];
                indices[i] = buckets[bi];
                buckets[bi]++;
            }

            size_t sum = 0;
            for( size_t i=0; i < 256; ++i )
            {
                size_t      &__b = buckets[i];
                const size_t __t = __b;
                __b  = sum;
                sum += __t;
            }

            size_t      j = primary_index;
            uint8_t    *c = buf_out + size;
            for( size_t i=size;i>0;--i)
            {
                const uint8_t bj = buf_in[j];
                *(--c) = bj;
                j = buckets[bj] + indices[j];
            }

        }

        void  bwt:: decode(void        *output,
                           void        *input,
                           const size_t size,
                           size_t      *indices,
                           const size_t primary_index,
                           shaker       &proc) throw()
        {
            assert(!(NULL==output&&size>0));
            assert(!(NULL==input&&size>0));

            size_t         buckets[] = {Y_BWT_Z256};         assert(256==sizeof(buckets)/sizeof(buckets[0]));
            uint8_t       *buf_in    = (uint8_t       *)input;
            uint8_t       *buf_out   = (uint8_t       *)output;

            for( size_t i=0; i < size; ++i )
            {
                const size_t bi = (buf_in[i]=proc.fetch(buf_in[i]));
                indices[i] = buckets[bi];
                buckets[bi]++;
            }

            size_t sum = 0;
            for( size_t i=0; i < 256; ++i )
            {
                size_t      &__b = buckets[i];
                const size_t __t = __b;
                __b  = sum;
                sum += __t;
            }

            size_t      j = primary_index;
            uint8_t    *c = buf_out + size;
            for( size_t i=size;i>0;--i)
            {
                const uint8_t bj = buf_in[j];
                *(--c) = bj;
                j = buckets[bj] + indices[j];
            }
        }

    }
}

