
#include "y/information/filter/shannon-fano.hpp"
#include "y/os/error.hpp"

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


            Alphabet::Node * Context:: Divide(Node *hi,
                                              size_t &nhi,
                                              Node *lo,
                                              size_t &nlo,
                                              const size_t n) throw()
            {
                assert(hi);
                assert(lo);
                assert(lo!=hi);
                assert(n>=2);
                assert(0==nhi);
                assert(0==nlo);
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
                        ++nhi;
                    }
                    else
                    {
                        break;
                    }
                }
                assert(nhi<n);
                nlo = n-nhi;
                return Dividing;
            }

            
            
            bool Context:: split(Node        *parent,
                                 const size_t length,
                                 size_t      &iNode) throw()
            {
                assert(parent);
                assert(Built==parent->symbol);
                assert(iNode<Nodes);
                assert(parent->prev);
                assert(parent->next);
                assert(parent->prev!=parent->next);
                assert(length>=2);
                
                const size_t bits = parent->bits+1;
                
                // check

                // build
                Node  *prev = parent->prev;
                Node  *next = parent->next;
                size_t nPrv = 0;
                size_t nNxt = 0;
                Node  *node = Divide(prev,nPrv,next,nNxt,length);
              
                
                

                return true;
            }

            void Context:: buildTree() throw()
            {
              
                assert(alpha.size>0);
                
                
                
                if(alpha.head==alpha.tail)
                {
                    assert(1==alpha.size);
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
                    split(root,alpha.size,iNode);
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

