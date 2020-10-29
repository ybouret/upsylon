
//! \file

#ifndef Y_PREFIX_NODE_INCLUDED
#define Y_PREFIX_NODE_INCLUDED 1

#include "y/associative/prefix/data.hpp"
#include "y/object.hpp"
#include "y/sort/merge.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    template <typename CODE, typename T>
    class prefix_node :
    public object,
    public prefix_data<T>,
    public ios::vizible
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
        inline explicit prefix_node(prefix_node *p,
                                    const CODE   c,
                                    T           *d) throw() :
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

        //! compare by decreasing frequency then increasing code
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

        virtual void vizCore(ios::ostream &fp) const
        {
            fp << "[label=\"";
            fp << prefix_::code_to_text(code);
            fp("#%lu [%lu]", (unsigned long)freq, (unsigned long)deep );
            fp << "\",shape=box";
            if(0!=this->used)
            {
                fp << ",style=filled";
            }
            fp << "]";
            endl(fp);
            for(const prefix_node *node=leaves.head;node;node=node->next)
            {
                node->vizSave(fp);
                endl(this->vizJoin(fp,node));
            }

        }
    };
    
}

#endif
