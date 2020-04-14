#include "y/jargon/grammar.hpp"
#include "y/type/aliasing.hpp"
namespace upsylon {
    
    namespace Jargon {

        XNode *Grammar:: AST( XNode *xnode ) const throw()
        {
            assert(xnode);
            
#if 0
            switch(xnode->genre)
            {
                case XNode::IsTerminal: return onTerminal(xnode);
                case XNode::IsInternal: return onInternal(xnode);
            }
#endif
            return xnode;
        }
        
        XNode * Grammar:: onTerminal(XNode *xnode) const throw()
        {
            //assert(xnode);
            //assert(xnode->isTerminal());
            //------------------------------------------------------------------
            //
            // pre-remove content
            //
            //------------------------------------------------------------------
            if( xnode->dogma->as<Terminal>().isDefinite() )
            {
                xnode->lexeme->release();
            }
            return xnode;
        }

        
        
        XNode *Grammar:: onInternal(XNode *xnode) const throw()
        {
            
            assert(xnode);
            XList &children = xnode->children;
            
            //------------------------------------------------------------------
            //
            // first pass: propagate and remove divisions
            //
            //------------------------------------------------------------------
            broadcast(children);
            
            //------------------------------------------------------------------
            //
            // second pass: study fusion
            //
            //------------------------------------------------------------------
            amalgamate(children);
            
            return xnode;
        }

        void Grammar:: broadcast(XList &children) const throw()
        {
#if 0
            XList temp;
            while(children.size)
            {
                XNode *chld = AST(children.pop_front());
                if(chld->isTerminal() && chld->dogma->as<Terminal>().isDivision())
                {
                    delete chld;
                }
                else
                {
                    temp.push_back(chld);
                }
            }
            children.swap_with(temp);
#endif
        }
        
        void Grammar:: amalgamate(XList &children) const throw()
        {
#if 0
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
                        case Aggregate::Steady:
                            temp.push_back(child);
                            break;
                            
                        case Aggregate::Acting: {
                            XList &sub = child->children;
                            if(1==sub.size)
                            {
                                temp.push_back( sub.pop_front() );
                                delete child;
                            }
                            else
                            {
                                temp.push_back(child);
                            }
                        } break;
                            
                        case Aggregate::Design:
                            temp.merge_back( child->children );
                            delete child;
                            break;
                    }
                }
                else
                {
                    temp.push_back(child);
                }
            }
            children.swap_with(temp);
#endif
        }
        
        
        void Grammar:: rewrite(XList &) const throw()
        {
            
        }
        
    }
    
}
