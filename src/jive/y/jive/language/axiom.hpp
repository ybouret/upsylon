//! \file

#ifndef Y_JIVE_LANG_AXIOM_INCLUDED
#define Y_JIVE_LANG_AXIOM_INCLUDED 1

#include "y/jive/language/node.hpp"

#include "y/ios/indent.hpp"
#include "y/momentary/increase.hpp"
#include "y/core/rnode.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            class Aggregate;
            class Terminal;
            
            //__________________________________________________________________
            //
            //! observing grammar run
            //__________________________________________________________________
            struct Observer
            {
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________

                //! base class to momentarily increase indentation
                typedef momentary_increase_by<1,unsigned> IncreaseType;

                //! local class to momentarily increase depth
                class Increase : public IncreaseType
                {
                public:
                    //! ++obs.depth
                    inline  Increase(Observer &obs) throw() : IncreaseType(obs.depth) {}
                    
                    //! --obs.depth
                    inline ~Increase() throw() {}

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Increase);
                };

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                ios::indent indent() const throw(); //!< to indent with depth
                
                void        mark(const Aggregate *) throw();
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                unsigned         depth;   //!< current depth
                const Aggregate *passed;  //!< last passed aggregate
                const Lexeme    *lexeme;  //!< last accepted lexeme
                const Lexeme    *tmplex;  //!< temporary lexeme
                const Aggregate *inside;  //!< last inside aggregate
                
            };

#define Y_LANG_AXIOM_ARGS   XNode * &tree, Source &source, Lexer &lexer, Observer &obs //!< arguments for accept()
#define Y_LANG_AXIOM_DECL()      virtual bool accept(Y_LANG_AXIOM_ARGS) const          //!< declare accept()
#define Y_LANG_AXIOM_IMPL(CLASS) bool CLASS:: accept(Y_LANG_AXIOM_ARGS) const          //!< implement accept()

            //__________________________________________________________________
            //
            //
            //! Axiom
            //
            //__________________________________________________________________
            class Axiom : public CountedObject, public inode<Axiom>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef core::list_of_cpp<Axiom>     List;      //!< alias
                typedef Axiom                       *Handle;    //!< alias
                typedef suffix_storage<Handle>       Registry;  //!< alias
                typedef ref_node<Axiom>              Reference; //!< alias
                typedef core::list_of_cpp<Reference> Manifest;  //!< alias

                static  bool                         Verbose;   //!< global language verbosity
                static  const char                   Accepted[];  //!< "ACCEPTED"
                static  const char                   Rejected[];  //!< "REJECTED"
                static  const char                  *Status(const bool) throw(); //!< accepted|rejected

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                string   gvName()  const; //!< create graphViz name
                virtual ~Axiom() throw(); //!< cleanup

                //! get derived class
                template <typename T> T &as() throw()
                {
                    assert(self); assert(uuid==T::UUID);
                    return *static_cast<T*>(self);
                }

                //! get derived class, const
                template <typename T> const T &as() const throw()
                {
                    assert(self); assert(uuid==T::UUID);
                    return *static_cast<T*>(self);
                }

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________

                //! accept method
                Y_LANG_AXIOM_DECL() = 0;
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag      name; //!< label
                const uint32_t uuid; //!< UUID

            protected:
                //! setup
                template <typename ID> inline
                explicit Axiom(const ID &i, const uint32_t t) : name( Tags::Make(i) ), uuid( t ), self(NULL) {}

                //! signature, mandatory in derived constructors
                template <typename T> inline
                void I_am() throw() { aliasing::_(self) = static_cast<T*>(this); }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Axiom);
                void    *self; //!< derived class pointer
            };

            //! message for verbosity
#define Y_LANG_PRINTLN(MSG) do { if(Jive::Language::Axiom::Verbose) { std::cerr << MSG << std::endl; } } while(false)

        }
    }
}

#endif

