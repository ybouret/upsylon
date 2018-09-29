//! \file
#ifndef Y_LANG_PATTERN_MATCHING
#define Y_LANG_PATTERN_MATCHING 1

#include "y/lang/pattern/compiler.hpp"
#include "y/container/sequence.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! matching a pattern against a string
        class Matching : public Token
        {
        public:
            virtual ~Matching() throw();    //!< destructor
            Matching( Pattern *p ) throw(); //!< initialize with a compiled pattern
            Matching( const string &rx, const Dictionary *dict=NULL); //!< init with regular expression
            Matching( const char   *rx, const Dictionary *dict=NULL); //!< init with regulat expresiion

            bool exactly( const string &s );                 //!< the string must entirely match the pattern
            bool partly(const string &s);                    //!< the string must partly match the pattern
            void find( sequence<Token> &, const string &s ); //!< find all consecutive, non overlapping occurences
            

        private:
            const arc_ptr<const Pattern> motif;
            Y_DISABLE_COPY_AND_ASSIGN(Matching);
        };

        //! wrapper to easy matching
        class MatchString : public Matching
        {
        public:
            //! initialize
            inline explicit MatchString(const char *rx) : Matching(rx,0) {}
            //! destructor
            inline virtual ~MatchString() throw() {}

            inline bool operator()( const string &s ) { return exactly(s); }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(MatchString);
        };

    }
}

#endif

