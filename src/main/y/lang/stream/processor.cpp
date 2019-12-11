

#include "y/lang/stream/processor.hpp"
#include "y/lang/pattern/compiler.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace  Lang {

        namespace Stream {

            Processor:: Processor() : codes()
            {
            }

            Processor:: ~Processor() throw()
            {
            }

            void Processor::on(const string &rx, const Instruction &I)
            {
                const Motif motif = RegExp(rx);
                if(motif->weak()) throw exception("weak pattern \"%s\" in Stream::Processor", *rx );
                const Code  code  = new Code_(motif,I);
                codes << code;
            }


            void Processor:: run( ios::ostream &target, Module *module )
            {
                Source source(module);

                const size_t numCodes = codes.size();
                Token        token;
                while( source.active() )
                {
                    // try code
                    bool match = false;
                    for(size_t i=1;i<=numCodes;++i)
                    {
                        Code_ &code = *codes[i];
                        token.release();
                        if(code.motif->match(token,source))
                        {
                            target << '<';
                            while(token.size)
                            {
                                target.write(token.head->code);
                                delete token.pop_front();
                            }
                            target << '>';
                            match = true;
                            break;
                        }
                    }


                    if(!match)
                    {
                        // no code
                        char C = 0;
                        if(!source.query(C))
                        {
                            throw exception("unexpected query failure");
                        }
                        target.write(C);
                    }


                }

            }


        }

    }
}

