
#ifndef Y_JARGON_PARSER_INCLUDED
#define Y_JARGON_PARSER_INCLUDED 1

#include "y/jargon/grammar.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! Parser from a Lexer and a Grammar
        /**
         The parser will automatically link the grammar Terminals and the
         lexical rules.
         */
        //
        //----------------------------------------------------------------------
        class Parser : public Lexer, public Grammar
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            virtual ~Parser() throw(); //!< cleanup
            
            //! setup
            template <typename LABEL> inline
            Parser( const LABEL &id ) :
            Lexer(id),
            Grammar(id),
            cache(),
            source(0)
            {
            }
            
            //------------------------------------------------------------------
            //
            // Terminal
            //
            //------------------------------------------------------------------
            
            //! create a terminal, detect univocal
            template <typename LABEL,typename REGEXP>
            Axiom & term(const LABEL      &label,
                          const REGEXP     &regexp)
            {
                // will emit the terminal
                const  Motif     &motif   = emit(label,regexp).motif; assert(motif->strong());
                Terminal::Feature feature = Terminal::Standard;
                if(motif->univocal())
                {
                    feature = Terminal::Univocal;
                }
                // and associate it with a terminal
                return terminal(label,feature,false);
            }
            
            
            //! create a terminal, wrapper
            template <typename LABEL>
            Axiom & term(const LABEL &both)
            {
                return term(both,both);
            }
            
            //------------------------------------------------------------------
            //
            // operators
            //
            //------------------------------------------------------------------
            
            //! create an operator terminal
            template <typename LABEL,typename REGEXP>
            Axiom & proc(const LABEL      &label,
                         const REGEXP     &regexp)
            {
                // will emit the terminal
                const  Lexical::Rule &rule  = emit(label,regexp);
                checkUnivocal(rule,"Operator");

                // and associate it with a terminal
                return terminal(label,Terminal::Univocal,true);
            }
            
            //! create an operator terminal, wrapper
            template <typename LABEL>
            Axiom & proc(const LABEL      &both)
            {
                return proc(both,both);
            }
            
            //------------------------------------------------------------------
            //
            // Division
            //
            //------------------------------------------------------------------
            
            //! create a division mark, must be univocal
            template <typename LABEL,typename REGEXP>
            Axiom & mark(const LABEL      &label,
                         const REGEXP     &regexp)
            {
                // will emit the terminal
                const  Lexical::Rule &rule  = emit(label,regexp);
                checkUnivocal(rule,"Terminal");
                // and associate it with a terminal
                return terminal(label,Terminal::Division,false);
            }
            
            //! create division mark, wrapper
            template <typename LABEL>
            Axiom & mark(const LABEL &both)
            {
                return mark(both,both);
            }
            
            //------------------------------------------------------------------
            //
            // Plugins
            //
            //------------------------------------------------------------------
            
            //! load a 0-arg plugin that will produce a terminal
            template <typename PLUGIN,typename LABEL>
            Axiom & plug(type2type<PLUGIN>,const LABEL &label)
            {
                load(type2type<PLUGIN>(),label).hook(*this);
                return terminal(label);
            }
            
            //! load a 1-arg plugin that will produce a terminal
            template <typename PLUGIN,typename LABEL,typename ENTER>
            Axiom & plug(type2type<PLUGIN>,const LABEL &label, const ENTER &enter)
            {
                load(type2type<PLUGIN>(),label,enter).hook(*this);
                return terminal(label);
            }
            
            //! load a 2-args plugin that will produce a terminal
            template <typename PLUGIN,typename LABEL,typename ENTER,typename LEAVE>
            Axiom & plug(type2type<PLUGIN>,const LABEL &label, const ENTER &enter, const LEAVE &leave)
            {
                load(type2type<PLUGIN>(),label,enter,leave).hook(*this);
                return terminal(label);
            }
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            
            //! validate grammar
            void compile();
            
            //! parsing a module, no restart
            XNode *parse( Module *, const bool doAST=true );
          
            //! parse a file
            template <typename FILENAME>
            XNode *parseFile(const FILENAME &fileName)
            {
                return parse( Module::OpenFile(cache,fileName) );
            }
            
            //! clean cache and xcache
            void cleanup() throw();
            
            //! load tree as Intermediate Language
            template <typename FILENAME>
            XNode *loadTreeFromFile( const FILENAME &filename )
            {
                Context ctx(filename);
                return treeFromFile(ctx);
            }
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            mutable Cache cache; //!< shared token cache

        protected:
            Source *source; //!< available source when parsing
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
            void checkUnivocal(const Lexical::Rule &, const char *which) const;
            XNode *treeFromFile(Context &ctx);
            
        };
        
    }
    
}
#endif
