
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
                class Scope
                {
                public:
                    Scope(Observer &obs, const Axiom *);
                    ~Scope() throw();

                private:
                    Observer &_;
                    Y_DISABLE_COPY_AND_ASSIGN(Scope);
                };

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                ios::indent indent() const throw(); //!< to indent with depth

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                unsigned         depth;   //!< current depth
                AxiomPool        apool;   //!< for memory

                explicit Observer() throw();
                virtual ~Observer() throw();

                void    call(const Axiom *);
                void    back() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Observer);
            };
        }
    }
}

#endif
