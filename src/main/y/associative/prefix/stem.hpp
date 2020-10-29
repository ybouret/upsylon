//! \file
#ifndef Y_PREFIX_STEM_INCLUDED
#define Y_PREFIX_STEM_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/object.hpp"
#include "y/sort/merge.hpp"

namespace upsylon {

    struct prefix_
    {
    public:
        static const char *byte_to_text(const uint8_t &) throw();

        template <typename T> static inline const char *code_to_text(const T &x) throw()
        {
            const union {
                T       _;
                uint8_t b[ sizeof(T) ];
            } alias = { x };
            clr();
            for(size_t i=0;i<sizeof(T);++i) cat( byte_to_text(alias.b[i]));
            return text;
        }

    private:
        static char text[];
        static void clr() throw();
        static void cat(const char *) throw();
    };

    template <typename T>
    class prefix_data
    {
    public:
        typedef unsigned_int<sizeof(void*)>::type flag_t;
        
        inline explicit prefix_data() throw()
        {
            used = 1;
            assert(NULL!=addr);
        }

        inline explicit prefix_data(T *p) throw()
        {
            addr = p;
            assert(addr!=NULL);
            assert(used!=0);
        }

        virtual ~prefix_data() throw()
        {
            used = 0;
            assert(NULL==addr);
        }

        union
        {
            T      *addr;
            flag_t  used;
        };

    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_data);

    };

    template <typename CODE, typename T>
    class prefix_node : public object, public prefix_data<T>
    {
    public:
        typedef prefix_data<T>                 data_t;
        typedef core::list_of<prefix_node>     list_t;
        typedef core::list_of_cpp<prefix_node> pool_t;

        //! initialize to a used node
        inline explicit prefix_node(const prefix_node *p,
                                    const CODE         c) throw() :
        object(),
        data_t(),
        code(c),
        next(0),
        prev(0),
        parent(p),
        leaves(),
        freq(0),
        deep(0)
        {
            initialize();
        }

        //! initialize to a used node
        inline explicit prefix_node(const prefix_node *p,
                                    const CODE         c,
                                    T                 *d) throw() :
        object(),
        data_t(d),
        code(c),
        next(0),
        prev(0),
        parent(p),
        leaves(),
        freq(0),
        deep(0)
        {
            initialize();
        }

        inline virtual ~prefix_node() throw()
        {
            while(leaves.size) delete leaves.pop_back();
        }

        CODE         code;
        prefix_node *next;
        prefix_node *prev;
        prefix_node *parent;
        list_t       leaves;
        size_t       freq;
        size_t       deep;

        inline void  optimize()              throw() { merging<prefix_node>::sort(leaves,compare,NULL); }
        inline void  leaves_to(pool_t &pool) throw() { while(leaves.size) pool.push_front(leaves.pop_back()); }
        inline void  return_to(pool_t &pool) throw() { leaves_to(pool);   pool.push_front(this);              }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_node);
        void   initialize() throw()
        {
            if(parent) deep = parent->deep + 1;

        }
        static inline int compare(const prefix_node *lhs,
                                  const prefix_node *rhs,
                                  void              *) throw()
        {
            const size_t lfreq = lhs->freq;
            const size_t rfreq = rhs->freq;
            if(lfreq<rfreq)
            {
                return 1;
            }
            else
            {
                if(rfreq<lfreq)
                {
                    return -1;
                }
                else
                {
                    assert(lfreq==rfreq);
                    assert(lhs->code!=rhs->code);
                    return comparison::increasing(lhs->code,rhs->code);
                }
            }
        }
    };

    

}

#endif

