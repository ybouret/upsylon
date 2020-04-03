#include "y/jargon/grammar.hpp"
#include "y/type/aliasing.hpp"
namespace upsylon {
    
    namespace Jargon {

        XNode *Grammar:: AST( XNode *xnode ) const throw()
        {
            assert(xnode);
            
            switch(xnode->genre)
            {
                case XNode::IsInactive: break;
                case XNode::IsTerminal: return onTerminal(xnode);
                case XNode::IsInternal: return onInternal(xnode);
            }
            return xnode;
        }
        
        XNode * Grammar:: onTerminal(XNode *xnode) const throw()
        {
            assert(xnode);
            assert(xnode->isTerminal());
            //------------------------------------------------------------------
            //
            // pre-remove content
            //
            //------------------------------------------------------------------
            if( xnode->dogma->as<Terminal>().isDefinite() )
            {
                aliasing::_(xnode->lexeme()).erase();
            }
            return xnode;
        }

        XNode *Grammar:: onInternal(XNode *xnode) const throw()
        {
            
            XList &children = xnode->children();
            
            //------------------------------------------------------------------
            //
            // first pass: propagate and remove divisions
            //
            //------------------------------------------------------------------
            {
                XList temp;
                while(children.size)
                {
                    XNode *chld = AST(children.pop_front());
                    if(chld->isTerminal() && chld->dogma->as<Terminal>().isDivision())
                    {
                        xcache.store(chld);
                    }
                    else
                    {
                        temp.push_back(chld);
                    }
                }
                children.swap_with(temp);
            }
            
            //------------------------------------------------------------------
            //
            // second pass: study fusion
            //
            //------------------------------------------------------------------
            {
                XList temp;
                while(children.size)
                {
                    XNode       *child = children.pop_front();
                    const Axiom &axiom = *(child->dogma);
                    if( Aggregate::UUID == axiom.uuid)
                    {
                        assert(child->isInternal());
                        switch( axiom.as<Aggregate>().feature )
                        {
                            case Aggregate::Permanent:
                                temp.push_back(child);
                                break;
                                
                            case Aggregate::Transient: {
                                XList &sub = child->children();
                                if(1==sub.size)
                                {
                                    temp.push_back( sub.pop_front() );
                                    xcache.store(child);
                                }
                                else
                                {
                                    temp.push_back(child);
                                }
                            } break;
                                
                            case Aggregate::Vanishing:
                                temp.merge_back( child->children() );
                                xcache.store(child);
                                break;
                        }
                    }
                    else
                    {
                        temp.push_back(child);
                    }
                }
                children.swap_with(temp);
            }
            
            return xnode;
        }

        
    }
    
}
