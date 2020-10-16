#include "y/associative/suffix/tree.hpp"
#include "y/exception.hpp"
#include "y/comparison.hpp"

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
        
        void suffix_tree:: rebuild_path(path &key, const node_type *curr)
        {
            key.free();
            assert(curr);
            while(curr->parent)
            {
                key.push_front(curr->code);
                curr=curr->parent;
            }
        }
        
        bool suffix_tree:: has( const void *key_addr, const size_t key_size ) const throw()
        {
            assert(!(NULL==key_addr&&key_size>0));
            const uint8_t *key = static_cast<const uint8_t *>(key_addr);
            return registered(key,key_size);
        }
        
        bool suffix_tree:: has(const char *text) const throw()
        {
            return has(text,text?strlen(text):0);
        }
        
        bool suffix_tree:: has(const memory::ro_buffer &buffer) const throw()
        {
            return has( buffer.ro(), buffer.length() );
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
                endl( fp( " [label=\"%s#%u%s\"]", cchars::printable[code], unsigned(freq),info) );
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
        
        static inline int compare_by_freq(const suffix_tree::node_type *lhs,
                                                const suffix_tree::node_type *rhs,
                                                void *) throw()
        {
            return comparison::decreasing(lhs->freq,rhs->freq);
        }
        
        void suffix_tree:: node_type:: trim() throw()
        {
            node_list temp;
            while(chld.size)
            {
                node_type *node = chld.pop_front();
                node->trim();
                if(0==node->impl&&node->chld.size<=0)
                {
                    delete node;
                }
                else
                {
                    temp.push_back(node);
                }
            }
            chld.swap_with(temp);
            freq = leaves();
            merging<node_type>::sort(chld,compare_by_freq,NULL);
        }
        
        void suffix_tree:: trim() throw()
        {
            root->trim();
        }
        
        size_t suffix_tree:: node_type:: leaves() const throw()
        {
            if(chld.size<=0)
            {
                return 1;//!< this is a leaf
            }
            else
            {
                size_t ans = 0;
                for( const node_type *node = chld.head; node; node=node->next )
                {
                    ans += node->leaves();
                }
                return ans;
            }
        }
        
        
        
    }
}

#include "y/string.hpp"

namespace upsylon {
    
    namespace core {
        
        upsylon::string suffix_tree:: path2string( const path &key )
        {
            size_t               n = key.size();
            upsylon::string      s(n,as_capacity,false);
            path::const_iterator k = key.begin();
            while(n-- >0 )
            {
                s << char(*(k++));
            }
            return s;
        }
    }
    
}
