#include "y/jargon/axiom/operator.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {

        const char Operator::CLID[] = "Operator";
        
        Operator:: ~Operator() throw()
        {
        }
        
        void Operator:: setup()
        {
            self = static_cast<Operator *>(this);
            Y_JAXIOM(std::cerr << "+[" << CLID << "] <" << label << ">" << std::endl);
        }
        
        bool Operator:: accept(Y_JARGON_AXIOM_ARGS_) const
        {
            Lexeme *lexeme = lexer.get(source);
            if( lexeme )
            {
                if( *(lexeme->label) == *label )
                {
                    throw exception("need to correct this...");
                    Y_JAXIOM(std::cerr << "|_[" << CLID << "] <" << label << ">" << std::endl);
                    //XNode::Advance(xtree,xcache.query(*this));
                    return true;
                }
                else
                {
                    lexer.unget(lexeme);
                    return false;
                }
            }
            else
            {
                // EOS
                return false;
            }
        }
        
        ios::ostream & Operator:: display(ios::ostream &fp) const
        {
            return fp("|_[%s<%s>]\n", CLID, **label);
        }
        
        const char *   Operator:: vizual() const throw()
        {
            return "shape=triangle,style=\"bold,filled\"";
        }
        
    }
    
}

