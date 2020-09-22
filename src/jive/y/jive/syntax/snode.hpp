//! \file
#ifndef Y_JIVE_SYNTAX_XNODE_INCLUDED
#define Y_JIVE_SYNTAX_XNODE_INCLUDED 1

#include "y/jive/lexemes.hpp"

namespace upsylon
{
    namespace Jive
    {
        
        namespace Syntax
        {
            class Axiom; //!< forward declaration



            //__________________________________________________________________
            //
            //
            //! flexible syntax node
            //
            //__________________________________________________________________
            class Node : public inode<Node>
            {
            public:
                //! Node classification
                enum  Kind
                {
                    IsTerminal, //!< for a  terminal node
                    IsInternal  //!< for an internal node
                };
                typedef core::list_of<Node> ListType;
                class List : public ListType
                {
                public:
                    explicit List() throw();
                    virtual ~List() throw();

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(List);
                };
                const Kind   kind;
                const Axiom &axiom;
                

            private:
                explicit Node(const Axiom&) throw();
                virtual ~Node() throw();
                
            public:
                typedef memory::tight::supply_of<Node> SupplyType; //!< alias

                //! todo
                class Supply : public singleton<Supply>, public SupplyType
                {
                public:
                    
                private:
                    Y_SINGLETON_DECL(Supply);
                    Y_DISABLE_COPY_AND_ASSIGN(Supply);
                    explicit Supply();
                    virtual ~Supply() throw();
                };
            };
            
        }
        
    }
}
    

#endif
