
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace  Syntax
        {
            
            Node * Node:: Load(Module *module, const Grammar &grammar)
            {
                assert(module);
                Source        source(module);
                size_t        shift;
                const string  which     = *grammar.name + " axiom name";
                const string  axiomName = string::read(source,shift,*which);
                const Axiom  *axiom     = grammar.query(axiomName);
            };
        }
        
    }
    
}

