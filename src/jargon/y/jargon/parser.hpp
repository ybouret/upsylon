
#ifndef Y_JARGON_PARSER_INCLUDED
#define Y_JARGON_PARSER_INCLUDED 1

#include "y/jargon/grammar.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! Parser from a Lexer and a Grammar
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
                return terminal(label,feature);
            }
            
            //! create a terminal, wrapper
            template <typename LABEL>
            Axiom & term(const LABEL &both)
            {
                return term(both,both);
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
                checkUnivocal(rule);
                // and associate it with a terminal
                return terminal(label,Terminal::Division);
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
            
            //! validate+build
            void compile();
            
            //! parsing a module
            XNode *parse( Module * );
            
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
            void checkUnivocal(const Lexical::Rule &) const;
        };
        
    }
    
}
#endif
