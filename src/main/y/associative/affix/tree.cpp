#include "y/associative/affix/tree.hpp"
#include "y/sort/merge.hpp"

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
        code(d)
        {

        }


        affix:: tree_node:: ~tree_node() throw()
        {
            assert(next==0);
            assert(prev==0);
            addr=0;
            freq=0;
            code=0;
        }


        static inline int by_decreasing_freq(const affix::tree_node *lhs,
                                             const affix::tree_node *rhs,
                                             void                   *) throw()
        {
            return (lhs->freq<rhs->freq) ? 1 : (rhs->freq<lhs->freq ? -1 : 0);
        }


        void affix:: tree_node:: optimize() throw()
        {
            merging<tree_node>::sort(leaves,by_decreasing_freq,NULL);
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
            pool.push_back(this);
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

        affix::tree_node * affix::new_tree_node(tree_node *parent, const uint8_t code)
        {
            assert(parent);
            if(tree_pool.size)
            {
                tree_node *node = tree_pool.pop_back();
                node->parent = parent;
                node->code   = code;
                node->freq   = 0;
                node->addr   = 0;
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
            root->code = 0;
        }

    }

}

#include <cstring>

namespace upsylon
{

    namespace core
    {

        bool affix:: insert_with(const char  *text,
                                 const size_t size,
                                 void *       addr)
        {
            assert(!(NULL==text&&size>0));
            return insert_path(text,size,addr);
        }


        bool affix:: insert_with(const char  *text,
                                 void *       addr)
        {
            return insert_path(text,text?strlen(text):0,addr);
        }

        bool affix:: insert_with(const memory::ro_buffer &buff, void *addr)
        {
            return insert_path( static_cast<const char *>( buff.ro() ), buff.length(), addr);
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
            fp("%lu", (unsigned long)freq );
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
