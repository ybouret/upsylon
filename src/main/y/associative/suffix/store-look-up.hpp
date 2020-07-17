
//! \file

#ifndef Y_SUFFIX_STORE_LOOK_UP_INCLUDED
#define Y_SUFFIX_STORE_LOOK_UP_INCLUDED 1

#include "y/core/list.hpp"


namespace upsylon {
    
    namespace core {
        
        //! algorithm to look up into a suffix store
        struct suffix_store
        {
            //! look up from a node
            template <
            typename T,
            typename ITERATOR,
            typename NODE>
            static inline
            bool look_up(const NODE *root,
                         ITERATOR    path_iter,
                         size_t      path_size) throw()
            {
                assert(root!=NULL);
                const NODE *curr = root;
                //------------------------------------------------------------------
                //
                // walk down
                //
                //------------------------------------------------------------------
                while(path_size-- > 0 )
                {
                    const T    code  = static_cast<const T>(*(path_iter++));
                    bool       found = false;
                    //--------------------------------------------------------------
                    //
                    // look for the code in child(ren)
                    //
                    //--------------------------------------------------------------
                    const core::list_of<NODE> &chld = curr->chld;
                    for(const NODE *node = chld.head;node;node=node->next)
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
                    // no code, no key
                    //
                    //--------------------------------------------------------------
                    if(!found)
                    {
                        return false;
                    }
                }
                assert(NULL!=curr);
                return curr->used;
            }
            
        };
    }
    
}

#endif

