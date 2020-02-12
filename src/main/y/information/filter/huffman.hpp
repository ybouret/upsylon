//! \file
#ifndef Y_INFORMATION_HUFFMAN_INCLUDED
#define Y_INFORMATION_HUFFMAN_INCLUDED 1

#include "y/information/qbits.hpp"
#include "y/information/filter/alphabet.hpp"
#include "y/ordered/priority-queue.hpp"
#include "y/information/filter/xqueue.hpp"

namespace upsylon {

    namespace information {


        struct Huffman
        {
            class Context : public Alphabet
            {
            public:
                typedef core::priority_queue<Node,Node::FrequencyComparator> PrioQ;

                virtual ~Context() throw();

                const Node &getRoot() const throw();

            protected:
                explicit Context(const Mode operating);
                void   buildTree() throw();
                void   restart() throw();
                
                Node  *root;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Context);
                PrioQ  pq;
                
            };

            class Encoder : public Context, public filterXQ
            {
            public:
                explicit Encoder( const Mode m);
                virtual ~Encoder() throw();

                virtual void reset() throw();
                virtual void write( char C );
                virtual void flush();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
            };

        };


        

    }

    
}

#endif
