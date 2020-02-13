#include "y/information/filter/huffman.hpp"

namespace upsylon {

    namespace information {

        Huffman::Decoder:: ~Decoder() throw()
        {
        }

        Huffman:: Decoder:: Decoder(const Mode m) :
        Context(m),
        flag(wait_for_byte),
        curr(root)
        {
        }

        void Huffman:: Decoder:: reset() throw()
        {
            setupTree();
            free_all();
            flag = wait_for_byte;
            curr = root;
        }

        void Huffman:: Decoder:: onNewByte( const uint8_t u )
        {
            push_back( u );
            (void) recv(u);
            buildTree();
            curr = root;
            flag = wait_for_bits;
        }


        void Huffman:: Decoder:: decode()
        {
            while(true)
            {
                switch(flag)
                {
                    case wait_for_byte:
                        assert(curr!=NULL);
                        assert(curr->symbol>=Chars);
                        if( io.size() < 8 )
                        {
                            return; // not enough bits
                        }
                        else
                        {
                            onNewByte( io.pop<uint8_t>() );
                        }
                        break;

                    case wait_for_bits:
                        assert(curr!=NULL);
                        assert(curr->symbol>=Chars);
                        assert(curr->left);
                        assert(curr->right);
                        if( io.size() <= 0 )
                        {
                            return;
                        }
                        else
                        {
                            curr                  = io.pop() ? curr->right : curr->left;
                            const CodeType symbol = curr->symbol;
                            if(symbol<=Codes)
                            {
                                switch(symbol)
                                {
                                    case EOS: io.drop(); curr=root; break;
                                    case NYT: flag = wait_for_byte; break;
                                    default: assert(symbol<Chars);
                                        onNewByte(symbol);
                                        break;
                                }
                            }
                        }
                        break;
                }
            }
        }


        void Huffman:: Decoder:: write(char C)
        {
            io.push(C);
            decode();
        }

        void Huffman:: Decoder:: flush()
        {
            
        }

    }

}

