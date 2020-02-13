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

        void Huffman:: Context:: setupTree() throw()
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
                const size_t nbit = node->bits;
                left->bits  = right->bits = nbit+1;
                left->code  = right->code = node->code;
                right->code |= (1<<nbit);
                UpdateCode(left);
                UpdateCode(right);
            }

        }

        void Huffman:: Context:: buildTree() throw()
        {
            assert(alpha.size>0);
            pq.clear();
            for(Node *node = alpha.tail;node;node=node->prev )
            {
                pq.enqueue(node);
                node->code = 0;
                node->bits = 1;
            }

            size_t iNode   = Codes;
            size_t maxBits = 0;
            while( pq.count > 1 )
            {
                assert(iNode<Nodes);
                Node *parent       = &nodes[iNode++]; assert(Built==parent->symbol);
                Node *right        = parent->right = pq.extract();
                Node *left         = parent->left  = pq.extract();
                const size_t nbits = parent->bits = max_of(left->bits,right->bits)+1;
                if(nbits>maxBits)
                {
                    maxBits = nbits;
                }
                parent->frequency  = left->frequency+right->frequency;

                pq.enqueue(parent);
            }
            assert(1==pq.count);
            //if(maxBits>16)
            {
                std::cerr << "maxBits=" << maxBits << std::endl;
            }
            root = pq.extract();

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
