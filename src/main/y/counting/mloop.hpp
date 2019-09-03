//! \file
#ifndef Y_COUNTING_MLOOP_INCLUDED
#define Y_COUNTING_MLOOP_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/type/args.hpp"
#include "y/memory/embed.hpp"

namespace upsylon
{

    template <typename T>
    class mloop : public counting
    {
    public:
        Y_DECL_ARGS(T,type);

        const size_t dimensions;

        inline explicit mloop(const size_t dim,
                              const_type  *ini,
                              const_type  *end) :
        counting(0),
        dimensions( chkdim(dim) ),
        curr(0),
        init(0),
        quit(0),
        move(0),
        iter(0),
        wksp(0),
        wlen(0)
        {
            assert(ini);
            assert(end);
            setup(ini,end);
        }

        inline virtual ~mloop() throw()
        {
            static memory::allocator &mem = mem_location();
            mem.release(wksp,wlen);
        }


        virtual void start() throw()
        {
            (size_t&)index = 1;
            for(size_t i=0;i<dimensions;++i)
            {
                curr[i] = init[i];
            }
        }

        virtual void next() throw()
        {
        }


        
    private:
        typedef void (*proc)(mutable_type &);
        mutable_type *curr;
        const_type   *init;
        const_type   *quit;
        const bool   *move;
        proc         *iter;
        void         *wksp;
        size_t        wlen;

        Y_DISABLE_COPY_AND_ASSIGN(mloop);

        inline void setup(const_type *ini, const_type *end)
        {
            static memory::allocator &mem = mem_instance();
            {
                memory::embed emb[] =
                {
                    memory::embed::as<mutable_type>(curr,dimensions),
                    memory::embed::as<const_type>  (init,dimensions),
                    memory::embed::as<const_type>  (quit,dimensions),
                    memory::embed::as<const bool>  (move,dimensions),
                    memory::embed::as<proc>        (iter,dimensions)
                };
                wksp = memory::embed::create(emb, sizeof(emb)/sizeof(emb[0]), mem, wlen);
            }
            for(size_t i=0;i<dimensions;++i)
            {
                const_type lo = ini[i];
                const_type up = end[i];
                *(mutable_type *) &init[i] = lo;
                *(mutable_type *) &quit[i] = up;
                *(bool         *) &move[i] = (lo!=up);
                *(proc         *) &iter[i] = (lo<up) ? incr : decr;
            }
        }

        static inline void incr(mutable_type&i) throw() { ++i; }
        static inline void decr(mutable_type&i) throw() { --i; }


    };


}

#endif

