
#include "y/information/filter/shannon-fano.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {

            Context:: ~Context() throw()
            {}


            Context:: Context(const Mode m) :
            Alphabet(m,Nodes*sizeof(Node*)),
            root(0),
            nstk( static_cast<Node *>( extra() ) )
            {
                assert( added >= Alive*sizeof(FreqType) );
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

            void Context:: split(Node *hi, Node *lo)
            {
                assert(hi);
                assert(lo);
                assert(hi!=lo);

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
                


            }

        }

    }
}

