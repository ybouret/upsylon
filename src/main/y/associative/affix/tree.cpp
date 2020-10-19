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


    }


}

namespace upsylon
{

    namespace core
    {
        affix:: affix() :
        root( new tree_node(0,0) ),
        data(),
        tree_pool(),
        data_pool()
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
            fp << "\",shape=box];\n";
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
