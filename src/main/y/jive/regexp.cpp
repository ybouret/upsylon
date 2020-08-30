#include "y/jive/pattern/all.hpp"
#include "y/jive/pattern/dictionary.hpp"
#include "y/ptr/auto.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    
    namespace Jive
    {
        
        namespace
        {
            
#define LPAREN '('
#define RPAREN ')'
            
#define Y_RX_PRINTLN(OUTPUT) \
do { if(RegExpCompiler::Verbose) { indent(std::cerr << "|_") << OUTPUT << std::endl; } } while(false)
            
            static const char fn[] = "Jive::RegExp: ";
            
            //==================================================================
            //
            // Regular Expression Compiler
            //
            //==================================================================
            class RegExpCompiler
            {
            public:
                
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                static bool        Verbose;
                const char        *curr; //!< current position
                const char        *last; //!< invalid position
                unsigned           rank; //!< current recursivity level
                const Dictionary  *dict; //!< optional dictionary
                const char * const expr; //!< initial expression
                
                inline std::ostream & indent(std::ostream &os) const {
                    unsigned r = rank; while(r-- >0) std::cerr << "__";
                    return os;
                }
                
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                
                //! cleanup
                inline ~RegExpCompiler() throw()
                {
                    curr = 0;
                    last = 0;
                    rank = 0;
                    dict = 0;
                }
                
                explicit RegExpCompiler(const char       *ini,
                                        const char       *end,
                                        const Dictionary *usr) :
                curr(ini),
                last(end),
                rank(0),
                dict(usr),
                expr(ini)
                {
                    
                }
                
                //--------------------------------------------------------------
                //
                //
                // main entry point: gathering patterns
                //
                //
                //--------------------------------------------------------------
                Logical * compile()
                {
                    //----------------------------------------------------------
                    //
                    // create a new pattern
                    //
                    //----------------------------------------------------------
                    Y_RX_PRINTLN("<compile@" << rank <<">");
                    auto_ptr<Logical> p = And::Create();
                    
                    //----------------------------------------------------------
                    //
                    // main RX loop
                    //
                    //----------------------------------------------------------
                    while(curr<last)
                    {
                        
                        const char C = *curr;
                        switch(C)
                        {
                                
                                //----------------------------------------------
                                //
                                // sub-expressions
                                //
                                //----------------------------------------------
                            case LPAREN:
                                //----------------------------------------------
                                Y_RX_PRINTLN("<sub-compile>");
                                //----------------------------------------------
                                ++curr;
                                ++rank;
                                p->push_back( compile() );
                                break;
                                
                            case RPAREN:
                                //----------------------------------------------
                                Y_RX_PRINTLN("<sub-compile/>");
                                //----------------------------------------------
                                --rank;
                                ++curr;
                                goto COMPILED;
                                
                                
                                
                                //----------------------------------------------
                                //
                                // default
                                //
                                //----------------------------------------------
                            default:
                                //----------------------------------------------
                                Y_RX_PRINTLN("<single '" << cchars::visible[uint8_t(C)] << "'>");
                                //----------------------------------------------
                                p->add(*(curr++));
                                break;
                        }
                        
                    }
                    
                COMPILED:
                    //----------------------------------------------------------
                    //
                    // end of current expression
                    //
                    //----------------------------------------------------------
                    Y_RX_PRINTLN("<compile@" << rank <<"/>");
                    if(p->size<=0)
                    {
                        throw exception("%sempty expression in '%s'",fn,expr);
                    }
                    return p.yield();
                }
                
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(RegExpCompiler);
            };
            
            //==================================================================
            
            bool RegExpCompiler::Verbose = true;
            
        }
        
        Pattern * RegExp(const string &rx, const Dictionary *dict)
        {
            const char       *ini = *rx;
            RegExpCompiler    rxc(ini,ini+rx.size(),dict);
            auto_ptr<Pattern> ptr = rxc.compile();
            if(rxc.rank!=0)
            {
                throw exception("%sunfinished expression '%s'",fn,ini);
            }
            return ptr.yield();
        }
        
        Pattern * RegExp(const char *text, const Dictionary *dict)
        {
            const string _(text);
            return RegExp(_,dict);
        }
        
        
    }
    
}

