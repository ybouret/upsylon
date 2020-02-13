//! \file
#ifndef Y_INFORMATION_HUFFMAN_INCLUDED
#define Y_INFORMATION_HUFFMAN_INCLUDED 1

#include "y/information/qbits.hpp"
#include "y/information/filter/alphabet.hpp"
#include "y/ordered/priority-queue.hpp"
#include "y/information/filter/xqueue.hpp"

namespace upsylon {

    namespace information {


        //! Huffman Codec
        struct Huffman
        {
            //! common operation: build the tree and the codes
            class Context : public Alphabet
            {
            public:
                //! prioririty queue to build tree
                typedef core::priority_queue<Node,Node::Priority> PrioQ;

                virtual    ~Context()       throw(); //!< cleanup
                const Node &getRoot() const throw(); //!< get the root node

            protected:
                explicit Context(const Mode operating); //!< setup
                void     buildTree() throw();           //!< build the tree
                void     setupTree() throw();           //!< initialize alphabet and build tree
                
                Node    *root; //!< the never NULL root!

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Context);
                PrioQ  pq;
                
            };

            //! encoder based on a context and an eXtended Queue Filter
            class Encoder : public Context, public filterXQ
            {
            public:
                explicit Encoder(const Mode m); //!< setup
                virtual ~Encoder() throw();     //!< cleanup

                virtual void reset() throw();   //!< restart context and free all data
                virtual void write(char C);     //!< encode char
                virtual void flush();           //!< flush with/out EOS

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
            };

            //! decoder based on a context and a queue filter
            class Decoder : public Context, public filterXQ
            {
            public:
                explicit Decoder(const Mode m); //!< setup
                virtual ~Decoder() throw();     //!< cleanup

                virtual void reset() throw();   //!< restart context and free data
                virtual void write(char C);     //!< write to io and try to decode
                virtual void flush();           //!< do nothing

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Decoder);
                enum Flag
                {
                    wait_for_byte,
                    wait_for_bits
                };
                Flag    flag;
                Node   *curr;

                void decode();
                void onNewByte( const uint8_t u );
            };


        };


        

    }

    
}

#endif
