
#include "y/jargon/lexical/plugin.hpp"

namespace upsylon {
    
    namespace Jargon
    {
        namespace Lexical {
            
            Plugin:: ~Plugin() throw()
            {
            }
            
            void Plugin:: initialize(const Token &token)
            {
                std::cerr << "Enter plugin |" << label << "| with '" << token << "'" << std::endl;
            }
        
            void Plugin:: hook( Scanner &scanner )
            {
                const Tag           ruleLabel = label;
                const Motif         ruleMotif     = RegularExpression::Compile(*trigger,dict_);
                const Action        action(this, &Plugin::initialize);
                const Event::Handle ruleEvent = new OnCall(action,ruleLabel);
                scanner.add( new Rule(ruleLabel,ruleMotif,ruleEvent) );
            }

        }
        
    }
    
}
