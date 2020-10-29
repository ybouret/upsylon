//! \file
#ifndef Y_PREFIX_STEM_INCLUDED
#define Y_PREFIX_STEM_INCLUDED 1

#include "y/associative/prefix/node.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! a stem is a basic dynamic tree
    //
    //__________________________________________________________________________
    template <typename CODE, typename T>
    class prefix_stem
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef          prefix_node<CODE,T>         node_type; //!< alias
        typedef typename prefix_node<CODE,T>::pool_t pool_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! setup with root node
        inline explicit prefix_stem() :
        root( new node_type(0,0) ), pool()
        {
        }

        //! cleanup
        inline virtual ~prefix_stem() throw() { delete root; root=0; }

        //______________________________________________________________________
        //
        //  methods
        //______________________________________________________________________

        //! get vizible root
        const ios::vizible & get_root() const throw() { assert(root); return *root; }

        //! tell number of inserted items
        inline size_t tell() const throw() { assert(root); return root->frequency; }


        //! grow current stem
        /**
         \param curr initial iterator, *curr must return a compatible type to CODE
         \param size size of path
         \param addr address to insert. TODO: NULL => used
         \param mark inserted node or busy node at path
         */
        template <typename ITERATOR>
        node_type *grow(ITERATOR    curr,
                        size_t      size,
                        T          *addr,
                        node_type **mark)
        {
            assert(0==root->code);
            assert(0==root->depth);
            //------------------------------------------------------------------
            // start from root
            //------------------------------------------------------------------
            node_type *node = root;
            while(size-- > 0)
            {
                //----------------------------------------------------------
                // look for code
                //----------------------------------------------------------
                const CODE     code = *(curr++);
                for(node_type *chld = node->leaves.head;chld;chld=chld->next)
                {
                    if(code==chld->code)
                    {
                        //------------------------------------------------------
                        // take this path
                        //------------------------------------------------------
                        node = chld;
                        goto FOUND;
                    }
                }
                //--------------------------------------------------------------
                // need a new leaf
                //--------------------------------------------------------------
                node = node->leaves.push_back( new_node(node,code) );
                assert(false==node->used);
            FOUND:;
            }
            assert(node!=NULL);
            if(mark) { *mark = node; }
            if(node->used)
            {
                //--------------------------------------------------------------
                // busy!
                //--------------------------------------------------------------
                return NULL;
            }
            else
            {
                //--------------------------------------------------------------
                // assign address and optimize
                //--------------------------------------------------------------
                node->addr = addr;
                increase_path_to(node);
                return node;
            }
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_stem);
        node_type *root;
        pool_type  pool;

        node_type *new_node(node_type *parent, const CODE code)
        {
            assert(parent);
            if(pool.size)
            {
                node_type *node = pool.pop_front();
                node->parent    = parent;
                node->code      = code;
                node->frequency = 0;
                node->depth     = parent->depth+1;
                node->used      = false;
                return node;
            }
            else
            {
                return new node_type(parent,code);
            }
        }

        void increase_path_to(node_type *node) throw()
        {
            assert(node);
            for(;;)
            {
                ++(node->frequency);
                node_type *parent = node->parent;
                if(parent)
                {
                    assert(parent->leaves.owns(node));
                    parent->optimize();
                    node=parent;
                }
                else
                {
                    assert(node==root);
                    return;
                }
            }
        }
    };

}

#endif

