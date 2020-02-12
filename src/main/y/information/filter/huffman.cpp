#include "y/information/filter/huffman.hpp"
//#include "y/code/utils.hpp"

namespace upsylon {

    namespace information {

        Huffman:: ~Huffman() throw()
        {
        }

        Huffman:: Huffman(const Mode opm) :
        Alphabet(Nodes,opm,Codes*sizeof(void*)),
        root(0),
        pq((Node **)extra,Codes)
        {
            buildTree();
        }

        void Huffman:: buildTree() throw()
        {
            assert(alpha.size>0);

            for(Node *node= alpha.head;node;node=node->next )
            {
                pq.enqueue(node);
            }

            size_t iNode = Codes;
            while( pq.count > 1 )
            {
                assert(iNode<Nodes);
                Node *parent = &nodes[iNode++]; assert(Built==parent->symbol);
                Node *left   = parent->left  = pq.extract();
                Node *right  = parent->right = pq.extract();
                parent->frequency = left->frequency+right->frequency;
                pq.enqueue(parent);
            }

            root = pq.extract();
        }

        const Alphabet::Node & Huffman:: getRoot() const throw()
        {
            assert(root);
            return *root;
        }
    }
}
