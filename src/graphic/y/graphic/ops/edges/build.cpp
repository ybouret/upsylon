
#include "y/graphic/ops/edges.hpp"
#include "y/sort/merge.hpp"

namespace upsylon {

    namespace Graphic {


        static inline
        int compareEdges( const Edge *lhs, const Edge *rhs, void * ) throw()
        {
            return comparison::decreasing(lhs->size,rhs->size);
        }

        void Edges:: build(size_t np)
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------

            Indices               &target = *this; target->clear();
            Edge::List            &edges  = *this; edges.release();
            const Pixmap<uint8_t> &source = L;

            assert(np<=source->items);

            const Point *points = &P[0][0];
            size_t       label  = 0;
            PPool        stack;

            //------------------------------------------------------------------
            //
            // loop on registered points
            //
            //------------------------------------------------------------------
            while( np-- > 0 )
            {
                const Point center = *(points++);
                assert(source[center]==STRONG || source[center]==FEEBLE);
                if(target[center]<=0)
                {
                    //----------------------------------------------------------
                    // start a new edge!
                    //----------------------------------------------------------
                    ++label;
                    Edge *edge = edges.push_back( new Edge(label) );

                    //----------------------------------------------------------
                    // explore
                    //----------------------------------------------------------
                    stack.store( new PNode( center ) );
                    while( stack.size>0 )
                    {
                        const Point current = **stack.head; assert( source[current]==STRONG || source[current]==FEEBLE );
                        size_t     &visited = target[current];
                        if( visited <= 0 )
                        {
                            //--------------------------------------------------
                            // not visited
                            //--------------------------------------------------

                            // update status if strong localMaxima
                            if(source[current]>=STRONG)
                            {
                                aliasing::_(edge->flag) = Edge::Strong;
                            }

                            // append this edge
                            edge->push_back( stack.query() );
                            visited = label;

                            // build probes
                            for(size_t i=0;i<8;++i)
                            {
                                const Point probe = current + Pixel::delta[i];
                                if(   target->contains(probe) // inside
                                   && (target[probe] <= 0)    // not visited
                                   && (source[probe] >  0)    // marked as localMaxima
                                   )
                                {
                                    // shall be included
                                    stack.store( new PNode(probe) );
                                }
                            }

                        }
                        else
                        {
                            //--------------------------------------------------
                            // already visited..
                            //--------------------------------------------------
                            assert(label==visited);
                            stack.pop();
                        }
                    }
                    assert(stack.size<=0);

                    if( Edge::Feeble == edge->flag )
                    {
                        for(const PNode *node = edge->head; node; node=node->next)
                        {
                            target[ **node ] = 0;
                        }
                        delete edges.pop_back();
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    // already got an edge
                    //----------------------------------------------------------
                    continue;
                }
            }

            // sort and relabel
            merging<Edge>::sort(edges, compareEdges, NULL);
            Edge *edge = edges.head;
            for(size_t i=1;i<=edges.size;++i,edge=edge->next)
            {
                aliasing::_(edge->label) = i;
                for(const PNode *node=edge->head;node;node=node->next)
                {
                    target[ **node ] = i;
                }
            }

        }

    }

}

