
//! \file

#ifndef Y_PREFIX_NODE_INCLUDED
#define Y_PREFIX_NODE_INCLUDED 1

#include "y/associative/prefix/data.hpp"
#include "y/object.hpp"
#include "y/sort/merge.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/ios/ostream.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! a node for stems
    //
    //__________________________________________________________________________
    template <typename CODE, typename T>
    class prefix_node :
    public object,
    public prefix_data<T>,
    public ios::vizible
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef prefix_data<T>                 data_t; //!< alias
        typedef core::list_of<prefix_node>     list_t; //!< low-level   list
        typedef core::list_of_cpp<prefix_node> pool_t; //!< hight-level pool
        typedef CODE                           code_t; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! initialize to a used=false
        inline explicit prefix_node(prefix_node *p,
                                    const CODE   c) throw() :
        object(),
        data_t(),
        code(c),
        next(0),
        prev(0),
        parent(p),
        leaves(),
        frequency(0),
        depth(0)
        {
            initialize();
        }

        //! initialize to a addr=d
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
        frequency(0),
        depth(0)
        {
            initialize();
        }

        //! cleanup
        inline virtual ~prefix_node() throw()
        {
            while(leaves.size) delete leaves.pop_back();
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! optimize to get a unique, fast tree
        inline void  optimize()              throw() { merging<prefix_node>::sort(leaves,compare,NULL); }

        //! return leaves to pool
        inline void  leaves_to(pool_t &pool) throw() { while(leaves.size) leaves.pop_back()->return_to(pool); }

        //! return leaves+this to pool
        inline void  return_to(pool_t &pool) throw() { leaves_to(pool); pool.push_front(this);                }
        
        //! build a path [1..depth]
        inline size_t encode( addressable<CODE> &path ) const throw()
        {
            assert(path.size()>=depth);
            const prefix_node *curr = this;
            while(curr->parent)
            {
                path[curr->depth] = curr->code;
                curr=curr->parent;
            }
            return depth;
        }

        //! build C-style path
        inline size_t encode(CODE *path) const throw()
        {
            const prefix_node *curr = this;
            path += depth;
            while(curr->parent)
            {
                *(--path) = curr->code;
                curr=curr->parent;
            }
            return depth;
        }

        //! check if both are free
        static bool are_both_free(const prefix_node *lhs, const prefix_node *rhs) throw()
        {
            assert(lhs); assert(rhs);
            return ((lhs->used==0) && (rhs->used==0));
        }
        
        //! check if both are used
        static bool are_both_used(const prefix_node *lhs, const prefix_node *rhs) throw()
        {
            assert(lhs); assert(rhs);
            return ((lhs->used!=0) && (rhs->used!=0));
        }
        
        //! both free or both used
        static bool have_same_status(const prefix_node *lhs, const prefix_node *rhs) throw()
        {
            assert(lhs); assert(rhs);
            return are_both_used(lhs,rhs) || are_both_free(lhs,rhs);
        }
        
        //! recursive check of same code,depths,status
        static bool have_same_layout(const prefix_node *lhs, const prefix_node *rhs) throw()
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
                    for(const prefix_node *l=L.head, *r=R.head;l;l=l->next,r=r->next)
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
        
        //! duplicate status BUT code
        void duplicate_status(const prefix_node *other) throw()
        {
            assert(other);
            assert(other->code==code);
            this->used = other->used;
            frequency  = other->frequency;
            depth      = other->depth;
        }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        CODE         code;      //!< the code inside the tree
        prefix_node *next;      //!< for list
        prefix_node *prev;      //!< for list
        prefix_node *parent;    //!< for tree
        list_t       leaves;    //!< low-level list of leaves
        size_t       frequency; //!< current frequency
        size_t       depth;     //!< current depth



    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_node);
        void   initialize() throw()
        {
            if(parent) depth = parent->depth + 1;

        }

        //! compare by decreasing frequency then increasing code
        static inline int compare(const prefix_node *lhs,
                                  const prefix_node *rhs,
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
            fp << prefix_::code_to_text(code);
            fp("#%lu [%lu]", (unsigned long)frequency, (unsigned long)depth );
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
