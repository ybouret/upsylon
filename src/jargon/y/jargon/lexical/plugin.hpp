//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_INCLUDED
#define Y_JARGON_LEXICAL_PLUGIN_INCLUDED 1

#include "y/jargon/lexical/scanner.hpp"

namespace upsylon {
    
    namespace Jargon
    {
        namespace Lexical
        {
            class Analyzer; //! forward declaration
            
            //! for template typing definitions
#define Y_JARGON_PLUGIN_DECL(TYPE) \
typedef type2type<TYPE> Type_;\
static const Type_      Type
            
            //! for template typing implementation
#define Y_JARGON_PLUGIN_IMPL(TYPE) \
const TYPE::Type_ TYPE::Type = {}

            //------------------------------------------------------------------
            //
            //! base class for a plugin
            //
            //------------------------------------------------------------------
            class Plugin : public Scanner
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~Plugin() throw(); //!< cleanup
              
                //--------------------------------------------------------------
                //
                // virtual interface
                //
                //--------------------------------------------------------------
                virtual void onInit( const Token & ) = 0;//!< called to initialize plugin
                
                
                //--------------------------------------------------------------
                //
                // non-virtual interface
                //
                //--------------------------------------------------------------
                void hook( Scanner &scanner ); //!< plugin is called upon trigger
                
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const Tag trigger;//!< regular expression that triggers the call

            protected:
                //! setup
                template <typename ID,typename EXPR>
                explicit Plugin(Analyzer         &Lx,
                                const ID         &id,
                                const EXPR       &regexp,
                                const EndOfStream eos) :
                Scanner(id,eos),
                trigger( Tags::Make(regexp) ),
                lexer(Lx)
                {
                    Y_JSCANNER(std::cerr << "|" << id << "| @'" << regexp << "'" << std::endl;);
                    plug_ = this;
                }
                                
                Analyzer &lexer; //!< used to insert compiled units
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
            };
        }
        
    }
    
}

#endif

