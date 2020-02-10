//! \file
#ifndef Y_LANG_PATTERN_DICTIONARY_INCLUDED
#define Y_LANG_PATTERN_DICTIONARY_INCLUDED 1

#include "y/lang/pattern.hpp"
#include "y/associative/map.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! dictionary aliases
        struct Dict
        {
            typedef arc_ptr<const Pattern>              Motif;    //!< alias
            typedef key_hasher<string,hashing::fnv>     Hasher;   //!< alias
            typedef memory::pooled                      Memory;   //!< alias
            typedef map<string,Motif,Hasher,Memory>     Patterns; //!< database
        };

        //! Dictionary of patterns for RegExp
        class Dictionary : public CountedObject, public Dict::Patterns
        {
        public:

            explicit Dictionary() throw();     //!< initialize
            explicit Dictionary(const size_t); //!< initialize with memory
            virtual ~Dictionary() throw();     //!< destructor

            //! register the pattern
            const Pattern &operator()(const string &name, Pattern *p);

            //! register the pattern, wrapper
            const Pattern &operator()(const char   *name, Pattern *p);

            //! with direct compilation
            const Pattern &operator()(const string &name, const string &rx);

            //! with direct compilation, wrapper
            const Pattern & operator()(const char *name, const char *rx);

            //! access to check
            const Pattern & operator[](const string &name) const;

            //! access to check, wrapper
            const Pattern & operator[](const char   *name) const;

            //! get and clone pattern
            Pattern * create(const string &name) const;

            //! get and clone pattern, wrapper
            Pattern * create(const char *name) const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dictionary);
        };
    }
}

#endif

