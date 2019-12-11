//! \file

#ifndef Y_LANG_STREAM_PROCESSOR_INCLUDED
#define Y_LANG_STREAM_PROCESSOR_INCLUDED 1

#include "y/lang/pattern.hpp"
#include "y/sequence/vector.hpp"
#include "y/functor.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon {

    namespace Lang {

        namespace Stream {

            typedef functor<bool,TL1(Token &)>  Editor; //!< interface

            //! apply editors
            class Processor
            {
            public:
                typedef arc_ptr<const Pattern>  Motif; //!< alias
                typedef arc_ptr<Editor>         Instr; //!< alias

                explicit Processor();          //!< setup
                virtual ~Processor() throw();  //!< cleanup

                void on(const string &rx, Editor *ed); //!< compile a code

                //! create and editor for a given regexp
                template <typename HOST, typename METHOD_POINTER> inline
                void on( const string &rx, HOST &host, METHOD_POINTER method )
                {
                    Editor *ed = new Editor( &host, method);
                    on(rx,ed);
                }

                //! create an editor for a given regexp
                template <typename HOST, typename METHOD_POINTER> inline
                void on( const char *rx, HOST &host, METHOD_POINTER method )
                {
                    const string _(rx);
                    on(_,host,method);
                }

                //! run module through codes
                void run( ios::ostream &target, Module *module );
                


            private:
                class Code
                {
                public:
                    Code(const Motif &M,
                         const Instr &I ) throw();
                    ~Code() throw();
                    Code(const Code &) throw();

                    const Motif motif;
                    const Instr instr;

                private:
                    Y_DISABLE_ASSIGN(Code);
                };

                vector<Code,memory::pooled> codes;
                Y_DISABLE_COPY_AND_ASSIGN(Processor);
            };



        }

    }

}

#endif

