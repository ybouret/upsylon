//! \file
#ifndef Y_JIVE_SYNTAX_AXIOM_INCLUDED
#define Y_JIVE_SYNTAX_AXIOM_INCLUDED 1

#include "y/jive/syntax/node.hpp"
#include "y/type/fourcc.hpp"
#include "y/core/rnode.hpp"

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

            class Aggregate;

            //! observer for error management
            struct  Observer
            {
                typedef const Aggregate *Whence;
                const Lexeme *lexeme;  //!< last accepted lexeme
                Whence        parent;  //!< address of named aggregate
                Whence        tested;  //!< currently tested
                
                //! update status
                void  mind(const Lexeme *now) throw();
            };

            //! args for accept method
#define Y_JIVE_AXIOM_ACCEPT_ARGS         XNode * & tree, Lexer &lexer, Source &source, Observer &guess, long depth

            //! accept declaration
#define Y_JIVE_AXIOM_ACCEPT_DECL()       virtual bool  accept_(Y_JIVE_AXIOM_ACCEPT_ARGS) const

            //! accept implemenation
#define Y_JIVE_AXIOM_ACCEPT_IMPL(CLASS)  bool CLASS:: accept_(Y_JIVE_AXIOM_ACCEPT_ARGS) const

            //! attach declaration
#define Y_JIVE_AXIOM_ATTACH_DECL()       virtual void attach(Axiom::Registry &) const

            
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
                static bool           Verbose;                       //!< verbosity level
                static const char     Prefix[];                      //!< "[JIVE] "
                typedef core::list_of_cpp<Axiom>     List;           //!< alias
                typedef Axiom                       *Pointer;        //!< alias
                typedef suffix_tree<Pointer>         RegistryType;   //!< alias
                typedef ref_node<const Axiom>        Reference;      //!< alias
                typedef core::list_of_cpp<Reference> Manifest;       //!< alias

                //______________________________________________________________
                //
                //! local Axiom registry
                //______________________________________________________________
                class Registry : public RegistryType
                {
                public:
                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    explicit Registry();                 //!< setup
                    virtual ~Registry() throw();         //!< cleanup
                    explicit Registry(const Registry &); //!< copy

                    //__________________________________________________________
                    //
                    // methods
                    //__________________________________________________________
                    std::ostream &        display(std::ostream&) const;                  //!< display
                    friend std::ostream & operator<<(std::ostream &, const Registry &);  //!< display
                    void                  ensure(const string &, Axiom &);               //!< register

                private:
                    Y_DISABLE_ASSIGN(Registry);
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
                bool         accept(Y_JIVE_AXIOM_ACCEPT_ARGS) const;
                bool         new_in(Axiom::Registry&)         const;  //!< check
                string       gvName()                         const;  //!< GraphViz Name
                Y_JIVE_AXIOM_ATTACH_DECL()                      = 0;  //!< recursive look up of all children
                void         called_by(const Axiom &parent)   const;  //!< modify 'from'
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const uint32_t      uuid; //!< identifier
                const Tag           name; //!< axiom name
                const void *const   self; //!< derived class
                const Registry      from; //!< called by

                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________
                //! grow a tree
                static void Grow( Node * &tree, Node *leaf ) throw();

                
                //! recall derived class
                template <typename CLASS>
                CLASS & as() throw()
                {
                    assert(NULL       !=self);
                    assert(CLASS::UUID==uuid);
                    return *static_cast<CLASS*>((void*)self);
                }

                //! recall derived class
                template <typename CLASS>
                const CLASS & as() const throw()
                {
                    assert(NULL       !=self);
                    assert(CLASS::UUID==uuid);
                    return *static_cast<const CLASS*>(self);
                }

                //! prepare named indentation
                std::ostream & indent(std::ostream &os, long depth) const;

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
#define Y_JIVE_PRINTLN(MSG) do { if(Verbose) { indent(std::cerr,depth) << MSG << std::endl; } } while(false)
        }
    }
}

#endif

