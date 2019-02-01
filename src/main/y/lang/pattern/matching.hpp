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
            Matching(const Matching &other) throw();                  //!< share the pattern

            bool exactly( const string &s );                 //!< the string must entirely match the pattern
            bool partly(const string &s);                    //!< the string must partly match the pattern
            size_t find( sequence<Token> &, const string &s ); //!< free and find all consecutive, non overlapping occurences
            
        private:
            const arc_ptr<const Pattern> motif;
            Y_DISABLE_ASSIGN(Matching);
        };

        //! wrapper to easy matching
        class MatchString : public Matching
        {
        public:
            //! initialize
            MatchString(const char   *rx);

            //! initialize
            MatchString(const string &rx);

            //! destructor
            virtual ~MatchString() throw();

            //! copy
            MatchString(const MatchString &other) throw();
            
            //! functionoid call
            bool operator()( const string &s );

            //! find all occurences
            size_t operator()( sequence<Token> &m, const string &s );

        private:
            Y_DISABLE_ASSIGN(MatchString);
        };

    }
}

#endif

