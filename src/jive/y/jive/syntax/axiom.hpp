//! \file
#ifndef Y_JIVE_SYNTAX_AXIOM_INCLUDED
#define Y_JIVE_SYNTAX_AXIOM_INCLUDED 1

#include "y/jive/syntax/node.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            //__________________________________________________________________
            //
            //
            // helpers
            //
            //__________________________________________________________________

            //! args for accept method
#define Y_JIVE_AXIOM_ACCEPT_ARGS        XNode * & tree, Lexer &lexer, Source &source

            //! accept declaration
#define Y_JIVE_AXIOM_ACCEPT_DECL()      virtual bool  accept_(Y_JIVE_AXIOM_ACCEPT_ARGS) const

            //! accept implemenation
#define Y_JIVE_AXIOM_ACCEPT_IMPL(CLASS)  bool CLASS:: accept_(Y_JIVE_AXIOM_ACCEPT_ARGS) const

            //__________________________________________________________________
            //
            //
            //! base class for Axiom
            //
            //__________________________________________________________________
            class Axiom : public CountedObject, public inode<Axiom>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const uint32_t TermUUID = Y_FOURCC('T','E','R','M'); //!< forward value
                static bool           Verbose;             //!< verbosity level
                static const char     Prefix[];            //!< "[JIVE] "
                typedef core::list_of_cpp<Axiom> List;     //!< alias
                typedef Axiom                   *Pointer;  //!< alias
                typedef suffix_tree<Pointer>     Registry; //!< alias
                
                //! lighweight reference for compoinds
                class Reference : public Object, public inode<Reference>
                {
                public:
                    typedef core::list_of_cpp<Reference> List;    //!< alias
                    explicit     Reference(const Axiom&) throw(); //!< setup
                    virtual     ~Reference() throw();             //!< cleanup
                    const Axiom &axiom;                           //!< internal reference

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Reference);
                };
                
                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                virtual ~Axiom()            throw(); //!< cleanup
                bool     isTerminal() const throw(); //!< uuid == TerminalUUID
                bool     isInternal() const throw(); //!< uuid != TerminalUUID

                //______________________________________________________________
                //
                //! accepting
                /**
                 - wrapper around accept_, to catch exceptions.
                 - if false => tree is untouched
                 - if true  => tree MAY change, may return empty sub-tree
                 */
                //______________________________________________________________
                bool     accept(Y_JIVE_AXIOM_ACCEPT_ARGS) const; //!< protected call to accept_

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const uint32_t    uuid; //!< identifier
                const Tag         name; //!< axiom name
                const void *const self; //!< derived class

                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________
                //! grow a tree
                static void Grow( Node * &tree, Node *leaf ) throw();

            protected:
                explicit Axiom(const uint32_t, const string *) throw(); //!< setup
                void     I_am(const void *)    throw();                 //!< set derived class
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Axiom);
                Y_JIVE_AXIOM_ACCEPT_DECL() = 0;
            };

            //! helper to link derived class to self
#define Y_JIVE_AXIOM(CLASS) I_am( static_cast<const CLASS *>(this) )

            //! helper
#define Y_JIVE_PRINTLN(MSG) do { if(Syntax::Axiom::Verbose) { std::cerr << Syntax::Axiom::Prefix << MSG << std::endl; } } while(false)
        }
    }
}

#endif

