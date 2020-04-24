
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
            template <typename LABEL> inline
            Axiom & mark(const LABEL &both)
            {
                return mark(both,both);
            }
            
            //------------------------------------------------------------------
            //
            // Plugins
            //
            //------------------------------------------------------------------
            
            //! load a 0-arg lexical plugin
            template <typename PLUGIN,typename LABEL> inline
            void grab(const type2type<PLUGIN> _,const LABEL &label)
            {
                load(_,label).hook(*this);
            }
            
            //! load a 1-arg lexical plugin
            template <typename PLUGIN,typename LABEL,typename ENTER> inline
            void grab(const type2type<PLUGIN> _,const LABEL &label, const ENTER &enter)
            {
                load(_,label,enter).hook(*this);
            }
            
            //! load a 2-args lexical plugin
            template <typename PLUGIN,typename LABEL,typename ENTER,typename LEAVE> inline
            void grab(const type2type<PLUGIN> _,const LABEL &label, const ENTER &enter, const LEAVE &leave)
            {
                load(_,label,enter,leave).hook(*this);
            }
            
            
            //! load a 0-arg plugin that will produce a terminal
            template <typename PLUGIN,typename LABEL> inline
            Axiom & plug(const type2type<PLUGIN> _,const LABEL &label)
            {
                grab(_,label);
                return _terminal(label,Terminal::Standard);
            }
            
            //! load a 1-arg plugin that will produce a terminal
            template <typename PLUGIN,typename LABEL,typename ENTER> inline
            Axiom & plug(const type2type<PLUGIN> _,const LABEL &label, const ENTER &enter)
            {
                grab(_,label,enter);
                return _terminal(label,Terminal::Standard);
            }
            
            //! load a 2-args plugin that will produce a terminal
            template <typename PLUGIN,typename LABEL,typename ENTER,typename LEAVE> inline
            Axiom & plug(const type2type<PLUGIN> _,const LABEL &label, const ENTER &enter, const LEAVE &leave)
            {
                grab(_,label,enter,leave);
                return _terminal(label,Terminal::Standard);
            }
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            
            //! retrieve pattern
            const Pattern &patternOf(const Axiom &axiom) const;
            
            //! validate grammar
            void compile();
            
            //! restart lexer and parse the module
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
            XNode *loadTreeFromFile( const FILENAME &fileName )
            {
                Context ctx(fileName);
                return treeFromFile(ctx);
            }
            
            //! load tree as Intermediate Language from data
            template <typename DATANAME>
            XNode *loadTreeFromData( const DATANAME &dataName, const void *data, const size_t size )
            {
                Context ctx(dataName);
                return treeFromData(ctx,data,size);
            }
            
            //! load tree as Intermediate Language from buffer
            template <typename DATANAME>
            XNode *loadTreeFromData( const DATANAME &dataName, const memory::ro_buffer &buff )
            {
                return loadTreeFromData(dataName, buff.ro(), buff.length());
            }
            
            //! save all axiom patterns
            void graphVizPatterns(const string &fn) const;
            
            //! save all axiom patterns
            void graphVizPatterns(const char   *fn) const;
            
            
        protected:
            Source *source; //!< available source when parsing
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
            void checkUnivocal(const Lexical::Rule &, const char *which) const;
            XNode *treeFromFile(Context &ctx);
            XNode *treeFromData(Context &ctx, const void *data, const size_t size);
        };
        
    }
    
}
#endif
