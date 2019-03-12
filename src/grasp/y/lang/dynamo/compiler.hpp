//! \file
#ifndef Y_DYNAMO_COMPILER_INCLUDED
#define Y_DYNAMO_COMPILER_INCLUDED 1

#include "y/lang/dynamo/loader.hpp"
#include "y/hashing/mph.hpp"
#include "y/lang/syntax/analyzer.hpp"

namespace upsylon
{
    namespace Lang
    {

#if 0
        //! base class for data
        class DynamoInfo
        {
        public:
            typedef memory::pooled                  Memory;
            typedef key_hasher<string,hashing::fnv> KeyHasher;

            const Tag moduleID;
            explicit DynamoInfo(const Tag &id ) throw();
            virtual ~DynamoInfo() throw();
            DynamoInfo(const DynamoInfo &other) throw();

        private:
            Y_DISABLE_ASSIGN(DynamoInfo);
        };

        template <typename T>
        class DynamoRef : public DynamoInfo
        {
        public:
            Y_DECL_ARGS(T,type); //!< alias
            typedef set<string,DynamoRef,KeyHasher,Memory> Set;

            type &rule;

            inline  DynamoRef( const Tag &id, type &r ) throw() : DynamoInfo(id), rule(r) {}
            inline ~DynamoRef() throw() {}
            inline  DynamoRef( const DynamoRef &other ) throw() : DynamoInfo(other), rule(other.rule) {}
            const string & key() const throw() { return rule.name; }

        private:
            Y_DISABLE_ASSIGN(DynamoRef);
        };

        typedef DynamoRef<Syntax::Compound> DynamoRule;
        typedef DynamoRef<Syntax::Terminal> DynamoTerm;

        //! build an intermediate tree
        class DynamoCompiler : public DynamoLoader
        {
        public:
            typedef vector<const Tag,DynamoInfo::Memory> Modules;
            explicit DynamoCompiler();
            virtual ~DynamoCompiler() throw();
            void reset() throw();

            Syntax::Parser *compile( XNode &top );

            auto_ptr<Syntax::Parser> parser;
            DynamoRule::Set          rules;
            DynamoRule::Set          terms;
            Modules                  modules;

            void decl( XNode &node ); //!< populate rules and terms

            const hashing::mperf top_h; //!< mperf(top_kw)

            static const char   *top_kw[];

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoCompiler);

            void    declModule( XNode &node );
            string *getModuleName( const XNode *node ) const;
            void    declRule( XNode &node );
            
        };
#endif

        enum DynamoType
        {
            DynamoTerminal,
            DynamoInternal
        };

        class DynamoNode;
        typedef core::list_of_cpp<DynamoNode> DynamoList;

        class DynamoNode : public core::inode<DynamoNode>
        {
        public:
            const DynamoType type;
            const string     name;

            explicit DynamoNode(const string &id, const Lexeme &lx, const size_t nskip=0,const size_t ntrim=0);
            explicit DynamoNode(const string &id);
            virtual ~DynamoNode() throw();

            const string       & content()  const throw();
            DynamoList         & children() throw();
            const DynamoList   & children() const throw();

            std::ostream & display( std::ostream &os, int level ) const;

            static        std::ostream & Indent(std::ostream &, int level);
            friend inline std::ostream & operator<<( std::ostream &os, const DynamoNode &node )
            {
                return node.display(os,0);
            }
        private:
            void            *impl;
            Y_DISABLE_COPY_AND_ASSIGN(DynamoNode);
        };


        //! build an intermediate tree
        class DynamoCompiler : public DynamoLoader, public Syntax::Analyzer
        {
        public:
            explicit DynamoCompiler();
            virtual ~DynamoCompiler() throw();

            //! generate intermediate code from node
            DynamoNode *compile( const XNode &node );



            size_t created;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoCompiler);
            DynamoList           items;
            const hashing::mperf lxh;  //!< "module", "cid", "lid", "rs", "rx"


            //! called when a terminal is met
            virtual void onTerminal( const string &id, const Lexeme &lx );

            //! called when an internal is met
            virtual void onInternal( const string &id, const size_t  sz, const string *data);
        };

    }
}

#endif

