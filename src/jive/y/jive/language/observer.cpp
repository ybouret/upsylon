
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
            
            Observer:: Observer(const string &g) throw() :
            AggPool(),
            depth(0),
            gname(g)
            {
            }
            
            Observer:: ~Observer() throw()
            {
            }

            const Aggregate * Observer:: in() const throw()
            {
                return head ? head->addr : NULL;
            }

            void Observer:: call(const Aggregate *a)
            {
                assert(a);
                if(apool.size>0)
                {
                    store( apool.pop(a) );
                }
                else
                {
                    push(a);
                }
                ++depth;
            }
            
            void Observer::back() throw()
            {
                assert(size>0);
                apool.store( pop() );
                --depth;
            }

            void Observer:: free() throw()
            {
                while(size) apool.store( pop() );
                depth=0;
            }
            
            
            Observer:: Scope::  Scope(Observer &obs, const Aggregate *a) :
            observer(obs),
            withCall(a!=NULL)
            {
                if(withCall)
                {
                    observer.call(a);
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

#include "y/jive/language/axiom/compound/aggregate.hpp"

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

