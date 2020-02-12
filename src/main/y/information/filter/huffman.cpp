#include "y/information/filter/huffman.hpp"
#include "y/type/utils.hpp"

namespace upsylon {

    namespace information {

        Huffman:: Context:: ~Context() throw()
        {
        }

        Huffman:: Context:: Context(const Mode opm) :
        Alphabet(opm,Codes*sizeof(void*)),
        root(0),
        pq((Node **)extra(),Codes)
        {
            buildTree();
        }

        void Huffman:: Context:: restart() throw()
        {
            initialize();
            buildTree();
        }
        
        static inline void UpdateCode( Alphabet::Node *node ) throw()
        {
            assert(node);
            Alphabet::Node *left = node->left;
            if(left)
            {
                Alphabet::Node *right = node->right;
                assert(right);
                left->bits  = right->bits = node->bits+1;
                right->code = (left->code = node->code << 1) | 1;
                UpdateCode(left);
                UpdateCode(right);
            }

        }

        void Huffman:: Context:: buildTree() throw()
        {
            assert(alpha.size>0);
            //std::cerr << "Building tree from #" << alpha.size << std::endl;

            pq.clear();
            for(Node *node = alpha.tail;node;node=node->prev )
            {
                pq.enqueue(node);
                node->code = 0;
                node->bits = 1;
            }

            size_t iNode = Codes;
            while( pq.count > 1 )
            {
                assert(iNode<Nodes);
                Node *parent      = &nodes[iNode++]; assert(Built==parent->symbol);
                Node *right       = parent->right = pq.extract();
                Node *left        = parent->left  = pq.extract();
                parent->frequency = left->frequency+right->frequency;
                parent->bits      = max_of(left->bits,right->bits)+1;
                pq.enqueue(parent);
            }
            assert(1==pq.count);
            root = pq.extract();
            //std::cerr << "iNode=" << iNode << " / " << Nodes << std::endl;
            root->code = 0;
            root->bits = 0;
            UpdateCode(root);


        }

        const Alphabet::Node & Huffman:: Context:: getRoot() const throw()
        {
            assert(root);
            return *root;
        }
    }
}
