//! \file
#ifndef Y_DYNAMO_COMPILER_INCLUDED
#define Y_DYNAMO_COMPILER_INCLUDED 1

#include "y/lang/dynamo/loader.hpp"
#include "y/hashing/mph.hpp"

namespace upsylon
{
    namespace Lang
    {

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

            void declModule( XNode &node );
            string *getModuleName( const XNode *node ) const;
            
        };
        
    }
}

#endif

