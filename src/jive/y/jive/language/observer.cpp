
#include "y/jive/language/observer.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            ios::indent Observer::indent() const throw()
            {
                return ios::indent(depth,'.');
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
            _(obs)
            {
                _.call(axiom);
            }


            Observer:: Scope::  ~Scope() throw()
            {
                _.back();
            }

        }

    }

}

