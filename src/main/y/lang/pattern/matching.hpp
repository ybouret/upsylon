//! \file
#ifndef Y_LANG_PATTERN_MATCHING
#define Y_LANG_PATTERN_MATCHING 1

#include "y/lang/pattern/compiler.hpp"
#include "y/container/sequence.hpp"
#include "y/ptr/auto.hpp"

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
            Matching( const char   *rx, const Dictionary *dict=NULL); //!< init with regular expresiion
            Matching(const Matching &other) throw();                  //!< share the pattern

            bool         exactly( const string &s );                 //!< the string must entirely match the pattern
            bool         partly(const string &s);                    //!< the string must partly   match the pattern
            size_t       find( sequence<Token> &, const string &s ); //!< free and find all consecutive, non overlapping occurences
            const Token *first_in(const string &s);                  //!< first in string, NULL if none
            const arc_ptr<const Pattern> motif;                      //!< the compile pattern
        private:
            Y_DISABLE_ASSIGN(Matching);
        };

        //! wrapper to easy matching
        class MatchString : public Matching
        {
        public:
            MatchString(const char   *rx);             //!< initialize
            MatchString(const string &rx);             //!< initialize
            virtual ~MatchString() throw();            //!< destructor
            MatchString(const MatchString &) throw();  //!< copy
            
            //! functionoid call for exactly
            bool operator()( const string &s );

            //! call wrapper
            bool operator()( const char   *s );

            //! find all occurences
            size_t operator()( sequence<Token> &m, const string &s );

        private:
            Y_DISABLE_ASSIGN(MatchString);
        };

    }
}

#endif

