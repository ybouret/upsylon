//! \file

#ifndef Y_SUFFIX_NODE_INCLUDED
#define Y_SUFFIX_NODE_INCLUDED 1

#include "y/associative/suffix/defs.hpp"
#include "y/core/list.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/object.hpp"
#include "y/exception.hpp"
#include "y/sort/merge.hpp"
#include "y/sequence/addressable.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! fully qualified, dynamic suffix tree node
    //
    //__________________________________________________________________________
    template <typename CODE>
    class suffix_node : public object , public ios::vizible
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef suffix::flag_t                 flag_t; //!< alias
        typedef core::list_of<suffix_node>     list_t; //!< raw list
        typedef core::list_of_cpp<suffix_node> pool_t; //!< full list for cache


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        inline explicit suffix_node(suffix_node *p, const CODE c, void *a) throw() :
        object(), ios::vizible(),
        next(0),
        prev(0),
        parent(p),
        leaves(),
        frequency(0),
        depth(parent?parent->depth+1:0)
        {
            code = c;
            addr = a;
        }

        //! cleanup
        inline virtual ~suffix_node() throw()
        {
            while(leaves.size) delete leaves.pop_back();
        }

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________

        //! make a node within a tree using pool
        inline static
        suffix_node *make(pool_t      &pool,
                          suffix_node *parent,
                          const CODE   c,
                          void        *a)
        {
            assert(parent);
            if(pool.size)
            {
                suffix_node *node = pool.pop_front();
                node->parent      = parent;
                node->depth       = parent->depth+1;
                node->frequency   = 0;
                node->code        = c;
                node->addr        = a;
                return node;
            }
            else
            {
                return new suffix_node(parent,c,a);
            }
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! optimize to get a unique, fast tree
        inline void  optimize()              throw() { merging<suffix_node>::sort(leaves,compare,NULL); }

        //! return leaves to pool
        inline void  leaves_to(pool_t &pool) throw() { while(leaves.size) leaves.pop_back()->return_to(pool); }

        //! return leaves+this to pool
        inline void  return_to(pool_t &pool) throw() { leaves_to(pool); pool.push_front(this);                }

        //! build a path [1..depth]
        template <typename TARGET>
        inline size_t encode( addressable<TARGET> &path ) const throw()
        {
            assert(path.size()>=depth);
            const suffix_node *curr = this;
            while(curr->parent)
            {
                path[curr->depth] = TARGET(curr->code);
                curr=curr->parent;
            }
            return depth;
        }

        //! build C-style path
        template <typename TARGET>
        inline size_t encode(TARGET *path) const throw()
        {
            const suffix_node *curr = this;
            path += depth;
            while(curr->parent)
            {
                *(--path) = TARGET(curr->code);
                curr=curr->parent;
            }
            return depth;
        }

        //! format exception
        inline void format(exception &excp) const throw()
        {
            const suffix_node *curr = this;
            while(curr->parent)
            {
                excp.hdr("%s",suffix::code_to_text(curr->code));
                curr=curr->parent;
            }
        }

        //! check if both are free
        static bool are_both_free(const suffix_node *lhs, const suffix_node *rhs) throw()
        {
            assert(lhs); assert(rhs);
            return ((lhs->used==0) && (rhs->used==0));
        }

        //! check if both are used
        static bool are_both_used(const suffix_node *lhs, const suffix_node *rhs) throw()
        {
            assert(lhs); assert(rhs);
            return ((lhs->used!=0) && (rhs->used!=0));
        }

        //! both free or both used
        static bool have_same_status(const suffix_node *lhs, const suffix_node *rhs) throw()
        {
            assert(lhs); assert(rhs);
            return are_both_used(lhs,rhs) || are_both_free(lhs,rhs);
        }

        //! recursive check of same code,depths,status
        static bool have_same_layout(const suffix_node *lhs, const suffix_node *rhs) throw()
        {
            assert(lhs);
            assert(rhs);
            if( (lhs->code==rhs->code) && (lhs->depth==rhs->depth) && have_same_status(lhs,rhs) )
            {
                // same code, depth and status
                const list_t &L  = lhs->leaves;
                const list_t &R  = rhs->leaves;
                if(L.size==R.size)
                {
                    for(const suffix_node *l=L.head, *r=R.head;l;l=l->next,r=r->next)
                    {
                        assert(l); assert(r);
                        if( !have_same_layout(l,r) )
                        {
                            return false;
                        }
                    }
                    return true;
                }
                else
                {
                    // different child(ren)
                    return false;
                }
            }
            else
            {
                // code,depth or status (is|are) different
                return false;
            }
        }

        

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        union {
            CODE     code; //!< actual code
            flag_t   priv; //!< alignment
        };
        suffix_node *next;   //!< for list/pool
        suffix_node *prev;   //!< for list
        suffix_node *parent; //!< for tree
        union
        {
            flag_t used;     //!< used!=0 <=> addr!=0
            void  *addr;     //!< NULL, effective of in_use()
        };
        list_t leaves;       //!< child(ren)
        size_t frequency;    //!< current frequency
        size_t depth;        //!< current depth

    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_node);

        //! compare by decreasing frequency then increasing code
        static inline int compare(const suffix_node *lhs,
                                  const suffix_node *rhs,
                                  void              *) throw()
        {
            const size_t lfreq = lhs->frequency;
            const size_t rfreq = rhs->frequency;
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
            fp << suffix::code_to_text(code);
            fp("#%lu [%lu]", (unsigned long)frequency, (unsigned long)depth );
            fp << "\",shape=box";
            if(0!=this->used)
            {
                fp << ",style=filled";
            }
            fp << "]";
            endl(fp);
            for(const suffix_node *node=leaves.head;node;node=node->next)
            {
                node->vizSave(fp);
                endl(this->vizJoin(fp,node));
            }

        }
    };

}

#endif
