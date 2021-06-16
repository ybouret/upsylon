
//! \file

#ifndef Y_JIVE_LANG_OBSERVER_INCLUDED
#define Y_JIVE_LANG_OBSERVER_INCLUDED 1

#include "y/ios/indent.hpp"
#include "y/core/addr-pool.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            class                                Axiom;     //!< forward
            typedef core::addr_node<const Axiom> AxiomNode; //!< alias
            typedef core::addr_pool<const Axiom> AxiomPool; //!< alias

            //! observer of language parser
            class Observer : public AxiomPool
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                
                //! local scope
                class Scope
                {
                public:
                    Scope(Observer &obs, const Axiom *); //!< forward, call if axiom!=NULL
                    Scope(Observer &obs) throw();        //!< forward, no call
                    ~Scope() throw();                    //!< reverse action(s)

                private:
                    Observer  &observer;
                    const bool withCall;
                    Y_DISABLE_COPY_AND_ASSIGN(Scope);
                };
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Observer() throw(); //!< setup
                virtual ~Observer() throw(); //!< cleanup
                
            
                ios::indent indent() const throw(); //!< to indent with depth
                const char *inside() const throw(); //!< current call/nil
                void        call(const Axiom *);    //!< push new call, increase depth
                void        back() throw();         //!< pop last call, decrease depth
                
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                unsigned         depth;   //!< current depth
                AxiomPool        apool;   //!< for memory
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Observer);
            };
        }
    }
}

#endif
