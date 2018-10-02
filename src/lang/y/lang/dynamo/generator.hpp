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




            //! symbol for linking and emission
            class _Symbol : public CountedObject
            {
            public:
                typedef intr_ptr<string,_Symbol> Pointer; //!< alias
                const string        expr;   //!< parser's string
                const string        name;   //!< terminal's name
                const Rule         &rule;   //!< created rule
                const Origin        module; //!< syntax module

                //! initialize
                inline _Symbol(const string &data,
                               const string &id,
                               const Rule   &r,
                               const Origin &from ) : expr(data), name(id), rule(r), module(from) {}

                //! desctructor
                inline virtual ~_Symbol() throw() {}

                //! for intr_ptr
                inline const string &key() const throw() { return expr; }

                inline friend std::ostream & operator<<( std::ostream &os, const _Symbol &s )
                {
                    os << s.module << '_' << s.name << '=' << '<' << s.expr << '>';
                    return os;
                }
            private:
                Y_DISABLE_COPY_AND_ASSIGN(_Symbol);
            };
            typedef _Symbol::Pointer Symbol;
            typedef set<string,Symbol,KeyHasher,Memory> Symbols;



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

            Symbols              terminals;

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



        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);
            string LxrToRX(const Node *sub,const string &lxr, string &label) const;
        };


    }

}

#endif

