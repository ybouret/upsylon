
//! \file

#ifndef Y_SUFFIX_STORE_INCLUDED
#define Y_SUFFIX_STORE_INCLUDED 1

#include "y/core/list.hpp"
#include "y/type/args.hpp"
#include "y/object.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    template <typename T>
    class suffix_store
    {
    public:
        Y_DECL_ARGS(T,type);
        class node_type;
        typedef core::list_of_cpp<node_type> node_list;

        class node_type : public object
        {
        public:
            node_type  *next;
            node_type  *prev;
            node_list   chld;
            bool        used;
            const_type  code;

            inline explicit node_type(const_type args) throw() :
            next(0), prev(0), chld(), used(false), code(args)
            {
            }

            inline virtual ~node_type() throw()
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(node_type);
        };

        inline explicit suffix_store() :
        root( new node_type(0) ),
        nodes(0)
        {
        }


        inline virtual ~suffix_store() throw()
        {
            delete root;
            root = 0;
            aliasing::_(nodes) = 0;
        }

        template <typename ITERATOR>
        bool insert(ITERATOR     path_iter,
                    size_t       path_size)
        {
            assert(root!=NULL);
            node_type *curr = root;

            //------------------------------------------------------------------
            //
            // walk down
            //
            //------------------------------------------------------------------
            while(path_size-- > 0 )
            {
                const_type code = *(path_iter++);
                bool       found = false;
                //--------------------------------------------------------------
                //
                // look for the code in child(ren)
                //
                //--------------------------------------------------------------
                node_list     &chld = curr->chld;
                for(node_type *node = chld.head;node;node=node->next)
                {
                    if(code==node->code)
                    {
                        curr   = node;
                        found  = true;
                        break;
                    }
                }

                //--------------------------------------------------------------
                //
                // check if a new child is to be set
                //
                //--------------------------------------------------------------
                if(!found)
                {
                    curr = curr->chld.push_back( new node_type(code) );
                    ++aliasing::_(nodes);
                }
            }
            assert(NULL!=curr);
            if(curr->used)
            {
                return false;
            }
            else
            {
                curr->used = true;
                return true;
            }
        }

        


    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_store);
        node_type *root;
    public:
        const size_t nodes;
    };

}

#endif

