
#include "y/jive/language/observer.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            ios::indent Observer::indent() const throw()
            {
                return ios::indent(depth,' ');
            }
            
            Observer:: Observer() throw() :
            AxiomPool(),
            depth(0)
            {
            }
            
            Observer:: ~Observer() throw()
            {
            }
            
            void Observer:: call(const Axiom *axiom)
            {
                assert(axiom);
                if(apool.size>0)
                {
                    store( apool.pop(axiom) );
                }
                else
                {
                    push(axiom);
                }
                ++depth;
            }
            
            void Observer::back() throw()
            {
                assert(size>0);
                apool.store( pop() );
                --depth;
            }
            
            
            Observer:: Scope::  Scope(Observer &obs, const Axiom *axiom) :
            observer(obs),
            withCall(axiom!=NULL)
            {
                if(withCall)
                {
                    observer.call(axiom);
                }
                else
                {
                    ++observer.depth;
                }
            }
            
            Observer:: Scope::  Scope(Observer &obs) throw() :
            observer(obs),
            withCall(false)
            {
                ++observer.depth;
            }
            
            
            
            
            
            Observer:: Scope::  ~Scope() throw()
            {
                if(withCall)
                {
                    observer.back();
                }
                else
                {
                    --observer.depth;
                }
            }
            
        }
        
    }
    
}

#include "y/jive/language/axiom.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            const char * Observer:: inside() const throw()
            {
                if(size)
                {
                    return **(**head).name;
                }
                else
                {
                    return core::ptr::nil;
                }
            }
        }
    }
}

