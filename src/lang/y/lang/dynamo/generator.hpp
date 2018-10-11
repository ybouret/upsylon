//! \file

#ifndef Y_LANG_DYNAMO_GENERATOR_INCLUDED
#define Y_LANG_DYNAMO_GENERATOR_INCLUDED 1

#include "y/lang/syntax/parser.hpp"
#include "y/associative/set.hpp"
#include "y/sequence/pipe.hpp"
#include "y/ptr/intr.hpp"
#include "y/hashing/mph.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {
        //! base class to store symbols
        template <typename RULE_TYPE>
        class DynamoSymbol : public CountedObject
        {
        public:
            inline virtual ~DynamoSymbol() throw() {} //!< destructor
            RULE_TYPE       &rule;   //!< reference to a Syntax Rule
            const Origin     module; //!< with its origun

            //! display Module_name
            inline std::ostream & display( std::ostream &os ) const
            {
                return (os << module << '_' << rule.name);
            }

        protected:
            //! initialize
            explicit inline DynamoSymbol(RULE_TYPE &r, const Origin &o) throw() : rule(r), module(o) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoSymbol);
        };

        
        //! for internal run-time consistency checks
#define Y_DYNAMO_CHECK(CODE) do { if( !(CODE) ) throw Exception(fn,"%s","FAILED " #CODE); } while(false)

        //! Generate a parser from a Dynamo compiled AST
        class DynamoGenerator
        {
        public:
            typedef Syntax::Node                    Node; //!< alias
            typedef Syntax::Rule                    Rule; //!< alias
            typedef key_hasher<string,hashing::fnv> KeyHasher; //!< hash strings
            typedef memory::pooled                  Memory;    //!< internal memory

            //! internal exception
            class Exception : public exception
            {
            public:
                //! format
                explicit Exception(const char *fn,const char *fmt,...) throw() Y_PRINTF_CHECK(3,4);
                virtual ~Exception() throw();             //!< destructor
                virtual const char *what() const throw(); //!< return internal _what
                Exception( const Exception &) throw();    //!< copy

            private:
                Y_DISABLE_ASSIGN(Exception);
                char _what[64];
            };


            //! symbol for linking and emission of terminals
            class _Terminal : public DynamoSymbol<const Syntax::Rule>
            {
            public:
                typedef DynamoSymbol<const Syntax::Rule> BaseType; //!< alias
                typedef intr_ptr<string,_Terminal>       Pointer;  //!< alias

                const string expr;    //!< parser's string as key
                const bool   visible; //!< public terminal, to analyze

                //! initialize
                inline _Terminal(const string &data,
                                 const Rule   &r,
                                 const Origin &from,
                                 const bool    flag=true) :
                BaseType(r,from), expr(data), visible(flag) {}

                //! destructor
                inline virtual ~_Terminal() throw() {}

                //! for intr_ptr
                inline const string &key() const throw() { return expr; }

                //! output with expr
                inline friend std::ostream & operator<<( std::ostream &os, const _Terminal &s )
                {
                    if(!s.visible) os << '!';
                    return (s.display(os) << '(' << s.expr << ')');
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(_Terminal);
            };
            typedef _Terminal::Pointer Terminal; //!< alias
            typedef set<string,Terminal,KeyHasher,Memory> Terminals; //!< database of terminals

            //! internal symbol
            class _Internal : public DynamoSymbol<Syntax::Compound>
            {
            public:
                typedef DynamoSymbol<Syntax::Compound> BaseType; //!< alias
                typedef intr_ptr<string,_Internal>     Pointer;  //!< alias

                //! destructor
                inline virtual ~_Internal() throw() {}

                //! initialize
                inline explicit _Internal(Syntax::Compound &r,
                                          const Origin     &from) throw() :
                BaseType(r,from) {}

                //! for database
                inline const string &key() const throw() { return rule.name; }

                //! display
                inline friend std::ostream & operator<<( std::ostream &os, const _Internal &s )
                {
                    return s.display(os);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(_Internal);
            };
            typedef _Internal::Pointer                    Internal;  //!< alias
            typedef set<string,Internal,KeyHasher,Memory> Internals; //!< database of internals


            auto_ptr<Syntax::Parser>  parser;    //!< currently built parser
            bool                      verbose;   //!< verbose flag
            int                       level;     //!< for tree walking
            lstack<const Origin>      modules;   //!< stack of travelled modules

            //! indent for verbose
            std::ostream & indent() const;

            static const char   *ktop[]; //!< "RULE", "ALIAS", "LXR", "PLUGIN"
            const hashing::mperf htop;   //!< hashing ktop
            static const char   *kstr[]; //!< "RX", "RS", "OS"
            const hashing::mperf hstr;   //!< hashing kstr
            static const char   *klxr[]; //!< "drop", "endl", "comment"
            const hashing::mperf hlxr;   //!< hashing klxr
            static const char   *ksyn[]; //!< "AGG", "ALT", "OOM", "ZOM", "OPT", "ID"...
            const hashing::mperf hsyn;   //!< hashing ksyn
            
            Terminals            terminals; //!< table of accessible terminals
            Internals            internals; //!< table of accessible internals
            
            static const int IS_AGG = 0; //!< hsyn("AGG")
            static const int IS_ALT = 1; //!< hsyn("ALT")
            static const int IS_OOM = 2; //!< hsyn("OOM")
            static const int IS_OPT = 3; //!< hsyn("OPT")
            static const int IS_ZOM = 4; //!< hsyn("ZOM")
            static const int IS_ID  = 5; //!< hsyn("ID")
            static const int IS_RS  = 6; //!< hsyn("RS")
            static const int IS_OS  = 7; //!< hsyn("OS")
            static const int IS_RX  = 8; //!< hsyn("RX")

            //! initialize
            explicit DynamoGenerator();

            //! destructor
            virtual ~DynamoGenerator() throw();

            //! create the parser
            Syntax::Parser *create( Node &dynamo, const bool verbose_flag=false);

            //__________________________________________________________________
            //
            //
            // first pass functions
            //
            //__________________________________________________________________

            //! get module name and push it on modules, increase level
            string getModuleName(const Node &dynamo ) const;
            
            //! create top level structure
            void topLevel( Node &dynamo );

            //! create an alias
            void onAlias( const Node &node );

            //! check lexical helpers
            void onLxr( const Node &node );

            //! check plugin
            void onPlugin( const Node &node );

            //! check rule
            void onRule( const Node &node );


            //__________________________________________________________________
            //
            //
            // second pass functions
            //
            //__________________________________________________________________

            //! walk down from top
            void walkDown( const Node &dynamo );

            //! upgrade top level compounds
            void finalize() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);
            string LxrToRX(const Node *sub,const string &lxr, string &label) const;

            //! recursive fill
            void fill( Syntax::Compound &content, const Node *parent, const int p_code );

            //! creating a rule
            const Syntax::Rule & createRule(const Node   *node,
                                           const int     h);

            //! query a previously create rule
            const Syntax::Rule & getRuleID(const char *fn, const Node *node) const;

            //! query of create a rule from a raw/string, with the operator flg
            const Syntax::Rule & getRuleFromString( const string &rs, const bool isOp );

            //! create a new visible terminal from a regexp
            const Syntax::Rule & getRuleFromRegExp( const string &rx );


            //! checking a top-level compound
            void check_top_level( Syntax::Compound &r ) throw();

        };


    }

}

#endif

