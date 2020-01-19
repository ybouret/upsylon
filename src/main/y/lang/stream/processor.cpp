

#include "y/lang/stream/processor.hpp"
#include "y/lang/pattern/compiler.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"
#include "y/ios/osstream.hpp"

namespace upsylon {

    namespace  Lang {

        namespace Stream {


            Processor::Code:: ~Code() throw()
            {
            }

            Processor::Code:: Code( const Code &_ ) throw() :
            motif(_.motif),
            instr(_.instr)
            {
            }

            Processor:: Code:: Code( const Motif &M, const Instr &I) throw() :
            motif(M),
            instr(I)
            {
            }


            Processor:: Processor() : codes()
            {
            }

            Processor:: ~Processor() throw()
            {
            }

            void Processor::on(const string &rx, Editor *ed)
            {
                static const char fn[] = "Stream::Processor: ";
                if(!ed) throw exception("%sNULL editor for \"%s\"",fn,*rx);
                const Instr instr = ed;
                const Motif motif = RegExp(rx);
                if(motif->weak()) throw exception("%sweak pattern \"%s\"",fn,*rx );
                
                const Code code(motif,instr);
                codes << code;
            }


            void Processor:: run( ios::ostream &target, Module *module )
            {
                // safe guard module
                Source source(module);

                // prepare some memory
                const size_t numCodes = codes.size();
                Token        token;
                
                while( source.active() )
                {
                    // try code
                    bool match = false;
                    for(size_t i=1;i<=numCodes;++i)
                    {
                        const Code &code = codes[i];
                        token.release();
                        if(code.motif->match(token,source))
                        {
                            // modify token
                           (void)aliasing::_(*code.instr)(token);

                            // output token
                            while( token.size )
                            {
                                target.write(token.head->code);
                                delete token.pop_front();
                            }

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


            string Processor:: operator()( const string &input )
            {
                string        result;
                {
                    ios::osstream target(result);
                    run( target, Module::OpenData("user's string", input) );
                }
                return result;
            }


        }

    }
}

