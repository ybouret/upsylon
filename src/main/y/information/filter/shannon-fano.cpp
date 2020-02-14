
#include "y/information/filter/shannon-fano.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {

            Context:: ~Context() throw()
            {}


            Context:: Context(const Mode m) :
            Alphabet(m,0),
            root(0)
            {
                buildTree();
            }


            void  Context:: update( Node *node ) throw()
            {
                assert(node);
                assert( alpha.owns(node) );
                while(node->prev && node->prev->frequency<node->frequency )
                {
                    alpha.towards_head(node);
                }
            }

            static inline
            Alphabet::FreqType freqDiff(const Alphabet::FreqType L,
                                        const Alphabet::FreqType R) throw()
            {
                return (L<R) ? (R-L) : (L-R);
            }

            void Context:: split(Node *parent, size_t &iNode) throw()
            {
                assert(Built==parent->symbol);
                assert(iNode<Nodes);
                Node *hi = parent->prev;
                Node *lo = parent->next;
                if(hi!=lo)
                {
#ifndef NDEBUG
                    for(const Node *node=hi;node!=lo;node=node->next)
                    {
                        assert(node->frequency>=node->next->frequency);
                    }
#endif
                    // initialize
                    FreqType L = hi->frequency;
                    FreqType R = lo->frequency;
                    for(const Node *node = lo->prev; node != hi; node=node->prev )
                    {
                        R += node->frequency;
                    }
                    FreqType D = freqDiff(L,R);
                    Node    *N = hi;

                    // search limit
                    for( Node *node=hi->next;node!=lo;node=node->next)
                    {
                        L += node->frequency;
                        R -= node->frequency;
                        const FreqType Dtmp = freqDiff(L,R);
                        if(Dtmp<D)
                        {
                            D = Dtmp;
                            N = node;
                        }
                    }

                    
                    assert(iNode<Nodes); Node *left  = parent->left  = &nodes[iNode++];
                    assert(iNode<Nodes); Node *right = parent->right = &nodes[iNode++];
                    left->prev = hi;
                    left->next = N;

                    right->prev=N->next;
                    right->next=lo;

                }
                else
                {
                    parent->left = parent->right = 0;
                }

            }

            void Context:: buildTree() throw()
            {
                // initialize
                size_t iNode = Codes;
                root = &nodes[iNode++];
                root->prev = alpha.head;
                root->next = alpha.tail;

                // recursive call
                split(root,iNode);

            }


            void Context:: setupTree() throw()
            {
                initialize();
                buildTree();
            }

            const Alphabet::Node & Context:: getRoot() const throw()
            {
                assert(root);
                return *root;
            }

        }

    }
}

