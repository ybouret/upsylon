
//! \file

#ifndef Y_JIVE_LEXICAL_UNIT_INCLUDED
#define Y_JIVE_LEXICAL_UNIT_INCLUDED 1

#include "y/jive/token.hpp"

namespace upsylon {


    namespace Jive {

        namespace Lexical {

            //__________________________________________________________________
            //
            //
            //! a unit is produced by a lexical rule
            //
            //__________________________________________________________________
            class Unit : public Token, public Context, public inode<Unit>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
               
                //! dedicated smart pointer
                class Pointer : public ptr<Unit>
                {
                public:
                    explicit Pointer(Unit*) throw();     //!< setup
                    virtual ~Pointer() throw();          //!< cleanup
                    Unit    *yield() throw();            //!< yield content
                    Pointer & operator=(Unit *) throw(); //!< auto setup
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Pointer);
                    void zap() throw();
                };
                
                typedef core::list_of<Unit> ListType; //!< alias for Lexemes

                //! List, using Supply for operations
                class List : public ListType, public releasable
                {
                public:
                    explicit List() throw();
                    virtual ~List() throw();
                    virtual void release() throw();
                    
                private:
                    Y_DISABLE_ASSIGN(List);
                    void clr() throw();
                };
                
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static Unit *Create(const Context &, const Tag &); //!< using supply
                static void  Delete(Unit *) throw();               //!< using supply
                

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag label; //!< creating rule label
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Unit);
                virtual ~Unit() throw();                             //!< cleanup
                explicit Unit(const Context &, const Tag &) throw(); //!< setup
                
            public:
                typedef memory::tight::supply_of<Unit> SupplyType; //!< alias

                //__________________________________________________________________
                //
                //! thread safe specialized Char::Supply
                //__________________________________________________________________
                class Supply : public singleton<Supply>, public SupplyType
                {
                public:
                    Y_SINGLETON_DECL(Supply);                     //!< aliases
                    Unit *acquire(const Context &, const Tag &); //!< built from supply
                    void  release(Unit *) throw();               //!< return memory
                    void  reserve(size_t);                       //!< query from system
                    
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Supply);
                    explicit Supply();
                    virtual ~Supply() throw();
                };
            };

        }
    }

}

#endif
