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

#define Y_DYNAMO_CHECK(CODE) do { if( !(CODE) ) throw Exception(fn,"%s","FAILED " #CODE); } while(false)

        //! Generate a parser from a Dynamo compiled AST
        class DynamoGenerator
        {
        public:
            typedef Syntax::Node         Node; //!< alias

            class Exception : public exception
            {
            public:
                virtual ~Exception() throw();
                explicit Exception(const char *fn,const char *fmt,...) throw() Y_PRINTF_CHECK(3,4);
                virtual const char *what() const throw(); //!< return internal _what
                Exception( const Exception &) throw();

            private:
                Y_DISABLE_ASSIGN(Exception);
                char _what[64];
            };


            //! initialize
            explicit DynamoGenerator();

            //! destructor
            virtual ~DynamoGenerator() throw();

            

            
            typedef key_hasher<string,hashing::fnv>              KeyHasher; //!< hash strings
            typedef memory::pooled                               Memory;    //!< internal memory
           // typedef set<string,Symbol::Pointer,KeyHasher,Memory> Symbols;   //!< table of symbols



            auto_ptr<Syntax::Parser>  parser;    //!< currently built parser
            bool                      verbose;   //!< verbose flag
            int                       level;     //!< for tree walking
            lstack<const Origin>      modules;   //!< stack of travelled modules

            //! indent for verbose
            std::ostream & indent() const;

            static const char   *ktop[]; //!< "RULE", "ALIAS", "LXR", "PLUGIN"
            const hashing::mperf htop;   //!< hashing ktop
            static const char   *kstr[]; //!< "RX", "RS", "OS"
            const hashing::mperf hstr;   //!<  hashing kstr

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
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);
        };


    }

}

#endif

