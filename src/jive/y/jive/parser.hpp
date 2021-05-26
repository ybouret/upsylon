
//! \file

#ifndef Y_JIVE_PARSER_INCLUDED
#define Y_JIVE_PARSER_INCLUDED 1

#include "y/jive/syntax/grammar.hpp"

namespace upsylon
{
    
    namespace Jive
    {


        //______________________________________________________________________
        //
        //
        //! parser with Grammar ans Lexer
        //
        //______________________________________________________________________
        class Parser : public Syntax::Grammar, public Lexer
        {
        public:
            //__________________________________________________________________
            //
            // types and declarations
            //__________________________________________________________________
            typedef Syntax::Axiom     Axiom;      //!< alias
            typedef Syntax::Terminal  Terminal;   //!< alias
            typedef Syntax::Aggregate Aggregate;  //!< alias
            typedef Syntax::Alternate Alternate;  //!< alias
            typedef Syntax::Compound  Compound;   //!< alias
            typedef Syntax::Repeat    Repeat;     //!< alias
            typedef Syntax::Option    Option;     //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Parser() throw(); //!< cleanup

            //! setup
            template <typename ID> inline
            explicit Parser(const ID &id) :
            Syntax::Grammar(id),
            Lexer(name)
            {
            }


            //__________________________________________________________________
            //
            // terminals
            //__________________________________________________________________

            //! Standard/Univocal Terminal ID = RX
            template <typename ID,typename RX> inline
            const Terminal & terminal(const ID &id,
                                      const RX &rx)
            {
                return term(id,queryTypeOf(checkStrong(emit(id,rx))));
            }

            //! Standard/Univocal Terminal ID = ID
            template <typename ID> inline
            const Terminal & terminal(const ID &id)
            {
                return terminal(id,id);
            }

            //! Division ID = RX
            template <typename ID,typename RX> inline
            const Terminal & division(const ID &id,
                                      const RX &rx)
            {
                (void) checkStrong( emit(id,rx) );
                return term(id,Terminal::Division);
            }

            //! Division ID = ID
            template <typename ID> inline
            const Terminal & division(const ID &id)
            {
                return division(id,id);
            }

            //! use a plugin as terminal
            template <typename PLUGIN,typename ID>
            const Terminal & plugin(const ID &id)
            {
                return term(call(plug<PLUGIN>(id)).label,Terminal::Standard);
            }

            //! new line as a terminal
            template <typename ID,typename RX> inline
            const Terminal & new_line(const ID &id,
                                      const RX &rx)
            {
                return term(id,queryTypeOf(checkStrong(emit_endl(id,rx))));
            }

            //! end line as a division
            template <typename ID,typename RX> inline
            const Terminal & end_line(const ID &id,
                                      const RX &rx)
            {
                (void) checkStrong(emit_endl(id,rx));
                return term(id,Terminal::Division);
            }




            //__________________________________________________________________
            //
            // management
            //__________________________________________________________________

            
            //! parse using Grammar::run
            XNode *parse(Source &source,const size_t prefetch=0);

            //! parse using Grammar::run
            template <typename ID>
            XNode *parseFile(const ID &fileName, const size_t prefetch=0)
            {
                Source source( Module::OpenFile(fileName) );
                return parse(source,prefetch);

            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
            const Lexical::Rule & checkStrong(const Lexical::Rule &) const;
            Terminal::Type        queryTypeOf(const Lexical::Rule &) const;
        };
    }

}

#endif

