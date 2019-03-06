//! \file
#ifndef Y_LANG_PATTERN_DICTIONARY_INCLUDED
#define Y_LANG_PATTERN_DICTIONARY_INCLUDED 1

#include "y/lang/pattern.hpp"
#include "y/associative/map.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! Dictionary of patterns for RegExp
        class Dictionary : public CountedObject
        {
        public:
            typedef Pattern                            *Handle;    //!< alias
            typedef key_hasher<string,hashing::fnv>     KeyHasher; //!< alias
            typedef memory::pooled                      Memory;    //!< alias
            typedef map<string,Handle,KeyHasher,Memory> Map;       //!< database

            explicit Dictionary() throw(); //!< initialize
            virtual ~Dictionary() throw(); //!< destructor

            //! register the pattern
            const Pattern &operator()(const string &name, Pattern *p);

            //! register the pattern, wrapper
            const Pattern &operator()(const char   *name, Pattern *p);

            //! with direct compilation
            const Pattern &operator()(const string &name, const string &rx);

            //! with direct compilation, wrapper
            const Pattern & operator()(const char *name, const char *rx);

            //! get and clone pattern
            Pattern * create(const string &name) const;

            //! get and clone pattern, wrapper
            Pattern * create(const char *name) const;

            //! release all
            void release() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dictionary);
            Map           entries;
            Pattern::List patterns;
        };
    }
}

#endif

