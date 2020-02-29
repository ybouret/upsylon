#include "y/information/translator/shannon-fano/decoder.hpp"

namespace upsylon {

    namespace Information {

        namespace ShannonFano {

            const char * Decoder::family() const throw() { return FMID;      }
            const char * Decoder::name()   const throw() { return DecoderID; }

            Decoder:: Decoder() :
            Tree(),
            TranslatorQueue(8),
            Q(32),
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
                    assert(curr->heavy);
                    assert(curr->light);
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
                            assert(NULL!=curr->left);
                            assert(NULL!=curr->right);
                            if(Q.size()<=0)
                            {
                                return;
                            }
                            else
                            {
                                curr = ( Q.pop() ? curr->right : curr->left);
                                const Char *chr = curr->heavy;
                                if(chr==curr->light)
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

