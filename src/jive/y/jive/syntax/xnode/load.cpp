
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace  Syntax
        {
            
            Node * Node:: Load(Source &source, const Grammar &grammar)
            {
                 
                
                //--------------------------------------------------------------
                // load name
                //--------------------------------------------------------------
                size_t        shift;
                const string  which     = *grammar.name + " axiom name";
                const string  axiomName = string::read(source,shift,*which);
                const Axiom  *axiomAddr = grammar.query(axiomName);
                if(!axiomAddr) throw exception("%s has no Axiom <%s>",**grammar.name,*axiomName);
                const Axiom  &axiom     = *axiomAddr;
                
                //--------------------------------------------------------------
                // load marker
                //--------------------------------------------------------------
                uint8_t marker = 0xff;
                if( !source.query( (char&)marker ) )
                {
                    throw exception("%s: no marker for <%s>",**grammar.name,*axiomName);
                }
                switch(marker)
                {
                    case TerminalMarker: {
                        //------------------------------------------------------
                        // read #chars
                        //------------------------------------------------------
                        std::cerr << "Loading Terminal" << std::endl;
                        size_t num_chars = 0;
                        if(!source.query_upack(num_chars,shift))
                        {
                            throw exception("%s: cannot read #chars for <%s>",**grammar.name,*axiomName);
                        }
                        Pointer          ptr( Node::Acquire(axiom,new Lexeme(source.context(),axiom.name)));
                        Lexeme          &lex = aliasing::_(*(ptr->lexeme()));
                        while(num_chars-- > 0)
                        {
                            Char *ch = source.get();
                            if(!ch) throw exception("%s: missing char for <%s>",**grammar.name,*axiomName);
                            lex.push_back(ch);
                        }
                        return ptr.yield();
                        
                    } break;
                        
                    case InternalMarker: {
                        //------------------------------------------------------
                        // read #leaves
                        //------------------------------------------------------
                        size_t num_leaves = 0;
                        if(!source.query_upack(num_leaves,shift))
                        {
                            throw exception("%s: cannot read #leaves for <%s>",**grammar.name,*axiomName);
                        }
                        //------------------------------------------------------
                        // read recursive loading
                        //------------------------------------------------------
                        Pointer xnode( Node::Acquire(axiom) );
                        while(num_leaves-- > 0)
                        {
                            xnode->leaves().push_back( Load(source,grammar) );
                        }
                        return xnode.yield();
                    } break;
                        
                    default:
                        break;
                }
                throw exception("%s: invalid marker for <%s>",**grammar.name,*axiomName);
            };
        }
        
    }
    
}

