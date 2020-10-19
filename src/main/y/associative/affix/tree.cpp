#include "y/associative/affix/tree.hpp"
#include "y/sort/merge.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{

    namespace core
    {

        affix:: tree_node:: tree_node(tree_node *p, const uint8_t d) throw() :
        addr(0),
        next(0),
        prev(0),
        parent(p),
        leaves(),
        freq(0),
        deep(0),
        code(d)
        {
            if(parent) deep = parent->deep+1;
        }




        affix:: tree_node:: ~tree_node() throw()
        {
            assert(next==0);
            assert(prev==0);
            parent = 0;
            addr=0;
            freq=0;
            deep=0;
            aliasing::_(code)=0;
        }

        size_t  affix:: tree_node:: children() const throw()
        {
            return leaves.size;
        }

        static inline int compare_nodes(const affix::tree_node *lhs,
                                        const affix::tree_node *rhs,
                                        void                   *) throw()
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
                    return int(lhs->code)-int(rhs->code);
                }
            }
        }


        void affix:: tree_node:: optimize() throw()
        {
            merging<tree_node>::sort(leaves,compare_nodes,NULL);
        }

        void affix:: tree_node:: leaves_to(tree_list &pool) throw()
        {
            while(leaves.size)
            {
                tree_node *node = leaves.pop_back();
                node->return_to(pool);
            }
        }

        void affix:: tree_node:: return_to(tree_list &pool) throw()
        {
            leaves_to(pool);
            pool.push_front(this);
        }



    }


}


namespace upsylon
{

    namespace core
    {
        affix:: affix() :
        root( new tree_node(0,0) ),
        tree_pool()
        {
        }

        affix:: ~affix() throw()
        {
            assert(root);
            delete root;
            root = 0;
        }

        size_t affix:: entries() const throw()
        {
            return root->freq;
        }

        size_t   affix:: in_pool() const throw()
        {
            return tree_pool.size;
        }

        void affix:: gc(size_t nmax) throw()
        {
            if(nmax<=0 || nmax >= tree_pool.size )
            {
                tree_pool.release();
            }
            else
            {
                merging<tree_node>::sort_by_increasing_address(tree_pool);
                while(nmax-- > 0)
                {
                    assert(tree_pool.size>0);
                    delete tree_pool.pop_back();
                }
            }
        }


        affix::tree_node * affix::new_tree_node(tree_node *parent, const uint8_t code)
        {
            assert(parent);
            if(tree_pool.size)
            {
                tree_node *node = tree_pool.pop_front();
                node->parent            = parent;
                aliasing::_(node->code) = code;
                node->freq              = 0;
                node->deep              = parent->deep+1;
                node->addr              = 0;
                return node;
            }
            else
            {
                return new tree_node(parent,code);
            }
        }

        void affix:: clear() throw()
        {
            root->leaves_to(tree_pool);
            root->addr = 0;
            root->freq = 0;
            root->deep = 0;
        }

        void affix:: increase_path_to(tree_node *node) throw()
        {
            assert(node);
            for(;;)
            {
                ++(node->freq);
                tree_node *parent = node->parent;
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

        void affix:: remove_node(tree_node *node) throw()
        {
            assert(node);
            assert(node->addr);

            node->addr = 0;
            for(;;)
            {
                assert(node->freq>0);
                const size_t  frequency = --(node->freq);
                tree_node    *parent    = node->parent;
                if(parent)
                {
                    assert(parent->leaves.owns(node));
                    if(frequency<=0)
                    {
                        parent->leaves.unlink(node)->return_to(tree_pool);
                    }
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


    }

}

#include <cstring>

namespace upsylon
{

    namespace core
    {

        affix::tree_node* affix:: insert_with(const char  *text,
                                              const size_t size,
                                              void *       addr)
        {
            assert(!(NULL==text&&size>0));
            return insert_at_path(text,size,addr);
        }


        affix::tree_node* affix:: insert_with(const char  *text,
                                              void *       addr)
        {
            return insert_at_path(text,text?strlen(text):0,addr);
        }

        affix::tree_node* affix:: insert_with(const memory::ro_buffer &buff, void *addr)
        {
            return insert_at_path( static_cast<const char *>( buff.ro() ), buff.length(), addr);
        }

        const affix::tree_node * affix:: node_with(const char  *text,
                                                   const size_t size) const throw()
        {
            assert(!(NULL==text&&size>0));
            return node_at_path(text,size);
        }

        const affix::tree_node * affix:: node_with(const char  *text) const throw()
        {
            return node_at_path(text,text?strlen(text):0);
        }

        const affix::tree_node * affix:: node_with(const memory::ro_buffer &buff) const throw()
        {
            return node_at_path(static_cast<const char *>( buff.ro() ), buff.length());
        }



    }

    

}

#include "y/ios/ocstream.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{

    namespace core
    {

        void affix::tree_node:: graphViz(ios::ostream &fp) const
        {
            fp.viz(this) << "[label=\"";
            fp << cchars::printable[code] << "#";
            fp("%lu[%lu]", (unsigned long)freq, (unsigned long)deep );
            fp << "\",shape=box,style=";
            if(addr)
            {
                fp << "filled";
            }
            else
            {
                fp << "solid";
            }
            fp << "];\n";
            for(const tree_node *node=leaves.head;node;node=node->next)
            {
                node->graphViz(fp);
                fp.viz(this) << "->";
                fp.viz(node) << ";\n";
            }
        }

    }

}

#include "y/ios/tools/graphviz.hpp"
#include "y/string.hpp"
namespace upsylon
{

    namespace core
    {

        void affix:: graphViz(const upsylon::string &fileName) const
        {
            {
                ios::ocstream fp(fileName);
                fp << "digraph G {\n";
                root->graphViz(fp);
                fp << "}\n";
            }

            ios::GraphViz::Render(fileName);
        }

        void affix:: graphViz(const char *fileName) const
        {
            const upsylon::string _(fileName);
            graphViz(_);
        }

    }
    
}
#include "y/hashing/function.hpp"

namespace upsylon
{

    namespace core
    {
        void affix:: tree_node:: run(hashing::function &H) const throw()
        {
            H.run_type(code);
            for(const tree_node *node=leaves.head;node;node=node->next)
            {
                node->run(H);
            }
        }

        void affix:: hash_with(hashing::function &H) const throw()
        {
            H.set();
            root->run(H);
        }
    }

}

