//! \file
#ifndef Y_DYNAMO_SYMBOLS_INCLUDED
#define Y_DYNAMO_SYMBOLS_INCLUDED 1

#include "y/lang/syntax/grammar.hpp"
#include "y/lang/dynamo/types.hpp"

namespace upsylon {

    namespace Lang
    {

        typedef memory::pooled                  DynamoMemory; //!< internal memory type
        typedef key_hasher<string,hashing::fnv> DynamoHasher; //!< string hasher

        //! predefined set
        template <typename T> struct DynamoSetOf
        {
            typedef set<string,T,DynamoHasher,DynamoMemory> Type; //!< alias
        };

        //! predefined map
        template <typename T> struct DynamoMapOf
        {
            typedef map<string,T,DynamoHasher,DynamoMemory> Type; //!< alias
        };


        //! base class for symbols
        class DynamoInfo : public DynamoObject
        {
        public:
            static const unsigned Plugin=0x01; //!< arising from a plugin
            static const unsigned FromRS=0x02; //!< arising from a raw string
            static const unsigned FromRX=0x04; //!< arising from a regular expression

            typedef DynamoSetOf<DynamoInfo>::Type Set;  //!< database of symbols

            DynamoInfo(const DynamoInfo &) throw();     //!< no throw copy
            virtual ~DynamoInfo() throw();              //!< destructor
            const string &key() const throw();          //!< rule.name

            const Tag           from;                   //!< creator module
            const Syntax::Rule &rule;                   //!< the generic underlying rule
            unsigned            info;                   //!< some binary info


            //! output
            friend std::ostream & operator<<(std::ostream &,const DynamoInfo &);

        protected:
            //! setup
            explicit DynamoInfo( const Tag &, const Syntax::Rule &, const unsigned ) throw();

        private:
            Y_DISABLE_ASSIGN(DynamoInfo);
        };

        //! base class to collect generic terminals and internals
        class DynamoSymbols : public DynamoObject
        {
        public:
            explicit DynamoSymbols() throw(); //!< setup
            virtual ~DynamoSymbols() throw(); //!< desctructor

            DynamoInfo::Set terminals; //!< set of terminals
            DynamoInfo::Set internals; //!< set of internals

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoSymbols);
        };

        typedef DynamoInfo::Set::iterator DynamoSymbolIterator; //!< alias

        //! generic derived symbol store for toplevel rules
        template <typename T>
        class DynamoRef : public DynamoInfo
        {
        public:
            typedef typename DynamoSetOf<DynamoRef>::Type Set; //!< specialized database

            T  &derived; //!< the derived type reference

            //! setup
            inline explicit DynamoRef(const Tag     &moduleID,
                                      T             &host,
                                      const unsigned flag  ) throw() :
            DynamoInfo(moduleID,host,flag), derived(host)
            {}

            //! desctructor
            inline virtual ~DynamoRef() throw() {}

            //! no-throw copy
            inline DynamoRef(const DynamoRef &other) throw() : DynamoInfo(other), derived(other.derived) {}

            //! display
            std::ostream & display( std::ostream &os ) const;

            //! output
            inline friend std::ostream & operator<<(std::ostream &os,const DynamoRef &dr)
            {
                return dr.display(os);
            }

        private:
            Y_DISABLE_ASSIGN(DynamoRef);
        };


        typedef DynamoRef<Syntax::Terminal> DynamoTerm; //!< alias for terminal
        typedef DynamoRef<Syntax::Compound> DynamoRule; //!< alias for compound

    }

}


#endif

