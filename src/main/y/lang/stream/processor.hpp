//! \file

#ifndef Y_LANG_STREAM_PROCESSOR_INCLUDED
#define Y_LANG_STREAM_PROCESSOR_INCLUDED 1

#include "y/lang/pattern.hpp"
#include "y/sequence/vector.hpp"
#include "y/functor.hpp"

namespace upsylon {

    namespace Lang {

        namespace Stream {

            typedef arc_ptr<const Pattern>      Motif;
            typedef functor<bool,TL1(Token &)>  Instruction;

            class Code_ : public CountedObject
            {
            public:
                explicit Code_(const Motif       &M,
                               const Instruction &I) : motif(M), instr(I) {}

                virtual ~Code_() throw() {}


                const Motif       motif;
                const Instruction instr;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code_);
            };

            typedef arc_ptr<Code_> Code;


            class Processor
            {
            public:
                explicit Processor();
                virtual ~Processor() throw();

                void on( const string &rx, const Instruction &I );

                template <typename HOST, typename METHOD_POINTER>
                void on( const string &rx, HOST &host, METHOD_POINTER method )
                {
                    const Instruction instr( &host, method );
                    on(rx,instr);
                }



                void run( ios::ostream &target, Module *module );

            private:
                vector<Code> codes;

                Y_DISABLE_COPY_AND_ASSIGN(Processor);
            };



        }

    }

}

#endif

