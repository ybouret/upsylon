
//! \file

#ifndef Y_JIVE_LANG_OBSERVER_INCLUDED
#define Y_JIVE_LANG_OBSERVER_INCLUDED 1

#include "y/ios/indent.hpp"
#include "y/core/addr-pool.hpp"
#include "y/strfwd.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {
            class                                    Aggregate;     //!< forward
            typedef core::addr_node<const Aggregate> AggNode; //!< alias
            typedef core::addr_pool<const Aggregate> AggPool; //!< alias

            //__________________________________________________________________
            //
            //
            //! observer of language parser
            //
            //__________________________________________________________________
            class Observer : public AggPool
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
                    Scope(Observer &obs, const Aggregate *); //!< forward, call if axiom!=NULL
                    Scope(Observer &obs) throw();            //!< forward, no call
                    ~Scope() throw();                        //!< reverse action(s)

                private:
                    Observer  &observer;
                    const bool withCall;
                    Y_DISABLE_COPY_AND_ASSIGN(Scope);
                };
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Observer(const string &) throw(); //!< setup with grammar name
                virtual ~Observer()               throw(); //!< cleanup
                

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                ios::indent indent() const throw(); //!< to indent with depth
                const char *inside() const throw(); //!< helper: current call/nil

                void             call(const Aggregate *); //!< push new call, increase depth
                void             back() throw();          //!< pop last call, decrease depth
                void             free() throw();          //!< all back
                const Aggregate *in() const throw();      //!< current head address

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                unsigned         depth;   //!< current depth
                AggPool          apool;   //!< for memory
                const string    &gname;   //!< grammar name for error handling

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Observer);
            };
        }
    }
}

#endif
