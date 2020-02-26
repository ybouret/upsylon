#include "y/information/translator/huffman/decoder.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {


            const char * Decoder::family() const throw() { return FMID;      }
            const char * Decoder::name()   const throw() { return DecoderID; }

            Decoder:: Decoder() :
            Tree(),
            TranslatorQueue(),
            Q(),
            flag( waitForByte ),
            curr( root )
            {
            }

            Decoder::~Decoder() throw()
            {
            }

            void Decoder:: reset() throw()
            {
                setupTree();
                free();
                Q.free();
                flag = waitForByte;
                curr = root;
            }

            void Decoder:: emitByte( const uint8_t byte )
            {
                push_back(byte);
                inputByte(byte,NULL);
                flag = waitForBits;
                curr = root;
            }

            void Decoder:: flush()
            {

            }

            void Decoder:: write(char C)
            {
                // feed queue
                Q.write(C);

                // use bits
                while( true )
                {
                    assert(curr!=NULL);
                    switch(flag)
                    {
                        case waitForByte:
                            if(Q.size()<8)
                            {
                                return;
                            }
                            else
                            {
                                emitByte( Q.pop<uint8_t>() );
                            }
                            break;

                        case waitForBits:
                            assert(NULL==curr->chr);
                            assert(NULL!=curr->left);
                            assert(NULL!=curr->right);
                            if(Q.size()<=0)
                            {
                                return;
                            }
                            else
                            {
                                curr = ( Q.pop() ? curr->right : curr->left);
                                const Char *chr = curr->chr;
                                if(chr)
                                {
                                    assert(chr->symbol<Codes);
                                    switch(chr->symbol)
                                    {
                                        case NYT: flag=waitForByte;    break;
                                        case EOS: Q.drop(); curr=root; break;
                                        default:  assert(chr->symbol<Bytes);
                                            emitByte(chr->symbol);
                                            break;
                                    }
                                }
                            }
                            break;
                    }
                }

            }

        }

    }

}

