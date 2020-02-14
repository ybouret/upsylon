
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


            Alphabet::Node * Context:: Divide(Node *hi, Node *lo) throw()
            {
                assert(hi);
                assert(lo);
                assert(lo!=hi);
                std::cerr << "split from " << NameOf(hi->symbol) << " to " << NameOf(lo->symbol) << std::endl;
                
#ifndef NDEBUG
                for(const Node *node=hi;node!=lo;node=node->next)
                {
                    assert(node->frequency>=node->next->frequency);
                }
#endif
                // initialize
                FreqType Lsum = hi->frequency;
                FreqType Rsum = lo->frequency;
                for(const Node *node = lo->prev; node != hi; node=node->prev )
                {
                    Rsum += node->frequency;
                }
                FreqType DeltaSum = freqDiff(Lsum,Rsum);
                Node    *Dividing = hi;

                // search limit
                for( Node *node=hi->next;node!=lo;node=node->next)
                {
                    Lsum += node->frequency;
                    Rsum -= node->frequency;
                    const FreqType Dtmp = freqDiff(Lsum,Rsum);
                    if(Dtmp<DeltaSum)
                    {
                        DeltaSum = Dtmp;
                        Dividing = node;
                    }
                    else
                    {
                        break;
                    }
                }

                return Dividing;
            }

            Alphabet::Node  * Context:: MakeLeft(Node *parent, Node *prev, Node *node, size_t &iNode) throw()
            {
                if(node==prev)
                {
                    Node *left = parent->left = node;
                    left->prev = left->next   = 0;
                    return left;
                }
                else
                {
                    Node *left = parent->left = nodes+iNode++;
                    left->prev = prev;
                    left->next = node;
                    return split(left,iNode)  ? left : 0;
                }
            }

            Alphabet::Node  * Context:: MakeRght(Node *parent, Node *node, Node *next, size_t &iNode) throw()
            {
                if(node==next)
                {
                    Node *right = parent->right = node;
                    right->left = right->prev   = 0;
                    return right;
                }
                else
                {
                    Node *right = parent->right = nodes+iNode++;
                    right->prev = node;
                    right->next = next;
                    return split(right,iNode) ? right : 0;
                }
            }


            bool Context:: split(Node * parent, size_t &iNode) throw()
            {
                assert(parent);
                assert(Built==parent->symbol);
                assert(iNode<Nodes);
                assert(parent->prev);
                assert(parent->next);
                assert(parent->prev!=parent->next);

                const size_t bits = parent->bits+1;

                // check

                Node *prev = parent->prev;
                Node *next = parent->next;
                Node *node = Divide(prev,next);

                Node *left = MakeLeft(parent, prev, node, iNode);
                if(!left) return false;
                left->bits = bits;

                Node *right = MakeRght(parent, node, next, iNode);
                if(!right) return false;
                right->bits = bits;

                return true;
            }

            void Context:: buildTree() throw()
            {
                assert(alpha.size>0);

                if(alpha.head==alpha.tail)
                {
                    root = alpha.head;
                    root->code = 0;
                    root->bits = 0;
                }
                else
                {
                    size_t iNode = Codes;
                    root = nodes+iNode;
                    root->prev = alpha.head;
                    root->next = alpha.tail;
                    root->code = 0;
                    root->bits = 0;
                    split(root,iNode);
                }

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

