#include "y/jargon/grammar.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        XNode *Grammar:: AST( XNode *xnode ) const throw()
        {
            return rewrite( compact(xnode) );
        }
        
        XNode *Grammar:: compact( XNode *xnode ) const throw()
        {
            assert(xnode);
            
            switch(xnode->dogma->uuid)
            {
                case Terminal::  UUID: return onTerminal(xnode);
                case Operator::  UUID: return onOperator(xnode);
                case Aggregate:: UUID: return onInternal(xnode);
                default:
                    break;
            }
            
            return xnode;
        }
        
        
        
        XNode * Grammar:: onTerminal(XNode *xnode) const throw()
        {
            assert(xnode);
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
        
        
        XNode * Grammar:: onOperator(XNode *xnode) const throw()
        {
            assert(xnode);
            //------------------------------------------------------------------
            //
            // pre-remove content
            //
            //------------------------------------------------------------------
            const Operator &op = xnode->dogma->as<Operator>();
            switch(op.attribute)
            {
                case Operator::Multiple: break;
                case Operator::Univocal: xnode->lexeme->release(); break;
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
            reduceAST(children);
            
            
            //------------------------------------------------------------------
            //
            // second pass: study fusion
            //
            //------------------------------------------------------------------
            fusionAST(children);
            
            
            return xnode;
        }
        
        void Grammar:: reduceAST(XList &children) const throw()
        {
            XList temp;
            while(children.size)
            {
                XNode       *child = compact(children.pop_front());
                const Axiom &axiom = *(child->dogma);
                if( Terminal::UUID == axiom.uuid && axiom.as<Terminal>().isDivision() )
                {
                    delete child;
                }
                else
                {
                    temp.push_back(child);
                }
            }
            children.swap_with(temp);
        }
        
        void Grammar:: fusionAST(XList &children) const throw()
        {
            XList temp;
            while(children.size)
            {
                XNode       *child = children.pop_front();
                const Axiom &axiom = *(child->dogma);
                if(Aggregate::UUID == axiom.uuid)
                {
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
        }
        
        
        XNode * Grammar:: rewrite(XNode *xnode) const throw()
        {
            if(Aggregate::UUID==xnode->dogma->uuid)
            {
                XList &children = xnode->children;
                {
                    XList temp;
                    while(children.size)
                    {
                        XNode       *child = rewrite(children.pop_front());
                        const Axiom &axiom = *(child->dogma);
                        if(Operator::UUID == axiom.uuid)
                        {
                            if(temp.size>0)
                            {
                                child->children.push_front(temp.pop_back());
                            }
                            if(children.size)
                            {
                                child->children.push_back( rewrite(children.pop_front()) );
                            }
                        }
                        temp.push_back(child);
                    }
                    children.swap_with(temp);
                }
                
                switch(xnode->dogma->as<Aggregate>().feature)
                {
                    case Aggregate::Steady: break;
                    case Aggregate::Acting: break;
                    case Aggregate::Design: break;
                }
                
                return xnode;
            }
            else
            {
                return xnode;
            }
        }
        
    }
    
}
