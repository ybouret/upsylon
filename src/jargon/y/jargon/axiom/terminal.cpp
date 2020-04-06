
#include "y/jargon/axiom/terminal.hpp"
#include "y/jargon/xcache.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {

        const char Terminal:: CLID[] = "Terminal";
        
        Terminal:: ~Terminal() throw()
        {
        }
        
        void Terminal:: setup()
        {
            self = static_cast<Terminal *>(this);
            //checkLabel(CLID);
            Y_JAXIOM(std::cerr << "+" << CLID << " <" << label << ">" << std::endl);
        }

        
        bool Terminal:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            assert(NULL==xtree||xtree->isInternal());
            Lexeme *lexeme = lexer.get(source);
            if( lexeme )
            {
                if( *(lexeme->label) == *label )
                {
                    Y_JAXIOM(std::cerr << "|_[" << CLID << "]<" << label << "> = '" << *lexeme << "'" << std::endl);
                    XNode::Advance(xtree,xcache.query(*this, lexeme));
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

        ios::ostream & Terminal:: display(ios::ostream &fp) const
        {
            return fp("|_[%s<%s>] $%s\n", CLID, **label,featureText());
        }

        bool Terminal:: isDefinite() const throw()
        {
            return Standard != feature;
        }
        
#define Y_JTERM_IS(EXPR) bool Terminal:: is##EXPR() const throw() { return EXPR == feature; }
        
        Y_JTERM_IS(Standard)
        Y_JTERM_IS(Univocal)
        Y_JTERM_IS(Division)

        const char * Terminal:: FeatureText(const Feature f)   throw()
        {
            switch (f) {
                case Standard: return "Standard";
                case Univocal: return "Univocal";
                case Division: return "Division";
            }
            return "???";
        }
        
        const char * Terminal:: featureText() const throw()
        {
            return FeatureText(feature);
        }
        
        
        
        
        const char *   Terminal:: vizual() const throw()
        {
            switch(feature)
            {
                case Univocal: return "shape=rectangle,style=\"bold,filled,rounded\"";
                case Division: return "shape=rectangle,style=\"dashed,bold,filled,rounded\"";
                default:
                    break;
            }
            return "shape=rectangle,style=\"bold,filled\"";
        }
    }
    
}


