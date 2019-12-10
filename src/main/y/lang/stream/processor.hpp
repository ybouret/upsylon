//! \file

#ifndef Y_LANG_STREAM_PROCESSOR_INCLUDED
#define Y_LANG_STREAM_PROCESSOR_INCLUDED 1

#include "y/lang/pattern.hpp"
#include "y/sequence/vector.hpp"
#include "y/functor.hpp"

namespace upsylon {

    namespace Lang {

        namespace Stream {

            typedef arc_ptr<const Pattern>                   Motif;
            typedef functor<void,TL2(const Token &,Source&)> Instruction;

            class Code_ : public CountedObject
            {
            public:
                explicit Code_(const Motif       &M,
                               const Instruction &I);

                virtual ~Code_() throw();

            private:
                const Motif       motif;
                const Instruction instr;
                Y_DISABLE_COPY_AND_ASSIGN(Code_);
            };

            typedef arc_ptr<Code_> Code;


            class Processor
            {
            public:
                explicit Processor();
                virtual ~Processor() throw();

                void on( const string &rx, const Instruction &I );

                void operator()( ios::ostream &dst, ios::istream &src );

            private:
                vector<Code> codes;

                Y_DISABLE_COPY_AND_ASSIGN(Processor);
            };



        }

    }

}

#endif

