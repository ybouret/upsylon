
#ifndef Y_JARGON_PARSER_INCLUDED
#define Y_JARGON_PARSER_INCLUDED 1

#include "y/jargon/grammar.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! Parser from a Lexer and a Grammar
        /**
         The parser will automatically link the grammar Terminals and Operators
         to their repective lexical rules.
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
            Parser( const LABEL &id ) : Lexer(id), Grammar(id), source(0)
            {
            }
            
            //------------------------------------------------------------------
            //
            // Terminals
            //
            //------------------------------------------------------------------
            
            //------------------------------------------------------------------
            //! create a Standard terminal, and detect Univocal
            //------------------------------------------------------------------
            template <typename LABEL,typename REGEXP>
            Axiom & term(const LABEL      &label,
                         const REGEXP     &regexp)
            {
                //______________________________________________________________
                //
                // will emit the terminal, check feature
                //______________________________________________________________
                const  Motif     &motif   = emit(label,regexp).motif; assert(motif->strong());
                Terminal::Feature feature = Terminal::Standard;
                if(motif->univocal())
                {
                    feature = Terminal::Univocal;
                }
                
                //______________________________________________________________
                //
                // and associate it with a terminal axiom
                //______________________________________________________________
                return _terminal(label,feature);
            }
            
            
            //------------------------------------------------------------------
            //! create a terminal, wrapper
            //------------------------------------------------------------------
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
            Axiom & op(const LABEL      &label,
                       const REGEXP     &regexp)
            {
                //______________________________________________________________
                //
                // will emit the terminal, check attribute
                //______________________________________________________________
                const  Motif        &motif     = emit(label,regexp).motif; assert(motif->strong());
                Operator::Attribute  attribute = Operator::Multiple;
                if(motif->univocal())
                {
                    attribute = Operator::Univocal;
                }
                //______________________________________________________________
                //
                // and associate it with a terminal
                //______________________________________________________________
                return _operator(label,attribute);
            }
            
            //------------------------------------------------------------------
            //! create an operator terminal, wrapper
            //------------------------------------------------------------------
            template <typename LABEL>
            Axiom & op(const LABEL      &both)
            {
                return op(both,both);
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
                //______________________________________________________________
                //
                // will emit the terminal
                //______________________________________________________________
                const  Lexical::Rule &rule  = emit(label,regexp);
                checkUnivocal(rule,TextFor::Terminal);
               
                //______________________________________________________________
                //
                // and associate it with a terminal
                //______________________________________________________________
                return _terminal(label,Terminal::Division);
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
                return _terminal(label,Terminal::Standard);
            }
            
            //! load a 1-arg plugin that will produce a terminal
            template <typename PLUGIN,typename LABEL,typename ENTER>
            Axiom & plug(type2type<PLUGIN>,const LABEL &label, const ENTER &enter)
            {
                load(type2type<PLUGIN>(),label,enter).hook(*this);
                return _terminal(label,Terminal::Standard);
            }
            
            //! load a 2-args plugin that will produce a terminal
            template <typename PLUGIN,typename LABEL,typename ENTER,typename LEAVE>
            Axiom & plug(type2type<PLUGIN>,const LABEL &label, const ENTER &enter, const LEAVE &leave)
            {
                load(type2type<PLUGIN>(),label,enter,leave).hook(*this);
                return _terminal(label,Terminal::Standard);
            }
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            
            //! validate grammar
            void compile();
            
            //! parsing a module, no restart
            XNode *parse( Module * );
            
            //! parse a file
            template <typename FILENAME>
            XNode *parseFile(const FILENAME &fileName)
            {
                return parse( Module::OpenFile(fileName) );
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
