#include "y/associative/suffix-tree.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace core {

        suffix_tree:: suffix_tree() : root( new node_type(0,0) )
        {
            
        }

        suffix_tree:: ~suffix_tree() throw()
        {
            delete root;
            root = 0;
        }

        void suffix_tree:: throw_on_insert_failure( const path &key) 
        {
            throw exception("unexpected suffix_tree insert failure w/key.size=%u", unsigned(key.size()));
        }
        
        void suffix_tree:: build_path_from( const node_type *curr, path &key )
        {
            key.free();
            assert(curr);
            while(curr->parent)
            {
                key.push_front(curr->code);
                curr=curr->parent;
            }
        }

        
        suffix_tree:: node_type:: ~node_type() throw() {}
        
        suffix_tree:: node_type::  node_type(node_type *from, const uint8_t value) throw() :
        object(),inode<node_type>(), ios::vizible(),
        parent(from), impl(0), code(value), freq(0), chld()
        {}
        
        void suffix_tree:: node_type:: vizCore( ios::ostream &fp ) const
        {
            const char *info = (impl!=0) ? "(*)" : "";
            if(parent)
            {
                endl( fp( " [label=\"%s#%u%s\"]", printable_char[code], unsigned(freq),info) );
            }
            else
            {
                endl(fp( " [label=\"root%s\"]",info));
            }
            for(const node_type *node=chld.head;node;node=node->next)
            {
                node->vizSave(fp);
                endl(vizJoin(fp,node));
            }
        }
        
    }
}

