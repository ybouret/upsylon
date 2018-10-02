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

        template <typename RULE_TYPE>
        class DynamoSymbol : public CountedObject
        {
        public:
            inline virtual ~DynamoSymbol() throw() {}
            const RULE_TYPE &rule;
            const Origin     module;

            inline std::ostream & display( std::ostream &os ) const
            {
                os << module << '_' << rule.name;
                return os;
            }

        protected:
            explicit DynamoSymbol( const RULE_TYPE &r, const Origin &o ) throw() :
            rule(r), module(o)
            {
            }

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
                virtual ~Exception() throw();             //!< destructor
                explicit Exception(const char *fn,const char *fmt,...) throw() Y_PRINTF_CHECK(3,4); //!< format
                virtual const char *what() const throw(); //!< return internal _what
                Exception( const Exception &) throw();    //!< copy

            private:
                Y_DISABLE_ASSIGN(Exception);
                char _what[64];
            };





            //! symbol for linking and emission of terminals
            class _Terminal : public DynamoSymbol<Syntax::Rule>
            {
            public:
                typedef intr_ptr<string,_Terminal> Pointer; //!< alias

                const string        expr;   //!< parser's string

                //! initialize
                inline _Terminal(const string &data,
                                 const Rule   &r,
                                 const Origin &from) :
                DynamoSymbol<Syntax::Rule>(r,from),
                expr(data) {}

                //! destructor
                inline virtual ~_Terminal() throw() {}

                //! for intr_ptr
                inline const string &key() const throw() { return expr; }

                inline friend std::ostream & operator<<( std::ostream &os, const _Terminal &s )
                {
                    s.display(os) << '(' << s.expr << ')';
                    return os;
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(_Terminal);
            };
            typedef _Terminal::Pointer Terminal; //!< alias
            typedef set<string,Terminal,KeyHasher,Memory> Terminals; //!< database of terminals

            class _Internal : public DynamoSymbol<Syntax::Compound>
            {
            public:
                typedef intr_ptr<string,_Internal> Pointer; //!< alias

                //! destructor
                inline virtual ~_Internal() throw() {}

                //! initialize
                inline explicit _Internal( const Syntax::Compound &r, const Origin &from ) throw() :
                DynamoSymbol<Syntax::Compound>(r,from)
                {
                }

                inline const string &key() const throw() { return rule.name; }

                inline friend std::ostream & operator<<( std::ostream &os, const _Internal &s )
                {
                    return s.display(os);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(_Internal);
            };
            typedef _Internal::Pointer Internal;                     //!< alias
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
            
            Terminals            terminals;
            Internals            internals;
            
            static const int IS_AGG = 0;
            static const int IS_ALT = 1;
            static const int IS_OOM = 2;
            static const int IS_OPT = 3;
            static const int IS_ZOM = 4;
            static const int IS_ID  = 5;
            static const int IS_RS  = 6;
            static const int IS_OS  = 7;


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

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);
            string LxrToRX(const Node *sub,const string &lxr, string &label) const;
        };


    }

}

#endif

