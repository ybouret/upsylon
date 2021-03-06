//! \file

#ifndef Y_JIVE_LANG_AXIOM_INCLUDED
#define Y_JIVE_LANG_AXIOM_INCLUDED 1

#include "y/jive/language/node.hpp"
#include "y/jive/language/observer.hpp"
#include "y/core/rnode.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            class   Compound;
            class   Terminal;
            class   Grammar;



#define Y_LANG_AXIOM_ARGS   XNode * &tree, Source &source, Lexer &lexer, Observer &obs //!< arguments for accept()
#define Y_LANG_AXIOM_DECL()      virtual bool accept(Y_LANG_AXIOM_ARGS) const          //!< declare accept()
#define Y_LANG_AXIOM_IMPL(CLASS) bool CLASS:: accept(Y_LANG_AXIOM_ARGS) const          //!< implement accept()

            //__________________________________________________________________
            //
            //
            //! Axiom
            //
            //__________________________________________________________________
            class Axiom : public Object, public dnode<Axiom>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef core::list_of_cpp<Axiom>        List;      //!< alias
                typedef Axiom                          *Handle;    //!< alias
                typedef suffix_storage<Handle>          Registry;  //!< alias
                typedef ref_dnode<Axiom>                Reference; //!< alias
                typedef core::list_of_cpp<Reference>    Manifest;  //!< alias
                typedef ref_snode<const Compound>       Host;      //!< alias
                typedef core::pool_of_cpp<Host>         Hosts;     //!< alias

                static  bool                         Verbose;                    //!< global language verbosity
                static  const char                   Accepted[];                 //!< "ACCEPTED"
                static  const char                   Rejected[];                 //!< "REJECTED"
                static  const char                  *Status(const bool) throw(); //!< accepted|rejected


                typedef Terminal                  *TermHandle;  //!< alias
                typedef suffix_storage<TermHandle> TermLedger_; //!< alias

                //______________________________________________________________
                //
                //! internal terminal ledger for first terminal detection
                //______________________________________________________________
                class TermLedger : public Object, public TermLedger_
                {
                public:
                    typedef core::pool_of_cpp<TermLedger> Pool; //!< alias

                    explicit TermLedger(const Compound &);      //!< setup
                    virtual ~TermLedger() throw();              //!< cleanup

                    TermLedger     *next; //!< for Pool
                    const Compound &from; //!< owner
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(TermLedger);
                };
                
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


                //! build the expecting first terminals
                static void Expecting(TermLedger  &first,
                                      const Axiom &axiom,
                                      unsigned     depth  =0,
                                      const bool   verbose=false);

                //! helper
                template <typename DERIVED>
                const DERIVED & to() const;


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
                const Tag            name;  //!< label
                const uint32_t       uuid;  //!< UUID
                const Hosts          hosts; //!< hosts

            protected:
                //! setup
                template <typename ID> inline
                explicit Axiom(const ID &i, const uint32_t t) :
                name( Tags::Make(i) ),
                uuid( t ),
                hosts(),
                self(NULL)
                {
                }

                //! signature, mandatory in derived constructors
                template <typename T> inline
                void I_am() throw() { aliasing::_(self) = static_cast<T*>(this); }

                //! low-level build the expecting first term from axiom
                static void ExpectingCore(TermLedger    &first,
                                          const Axiom   &axiom,
                                          Registry      &db,
                                          unsigned       depth,
                                          const bool     verbose);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Axiom);
                friend class Grammar;
                void         *self; //!< derived class pointer
            };

            //! message for verbosity
#define Y_LANG_PRINTLN(MSG) do { if(Jive::Language::Axiom::Verbose) { std::cerr << MSG << std::endl; } } while(false)

             
        }
    }
}

#endif

