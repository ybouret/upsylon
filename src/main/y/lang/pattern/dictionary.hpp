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
            typedef Pattern           *Handle; //!< alias
            typedef map<string,Handle> Map;    //!< database

            explicit Dictionary() throw(); //!< initialize
            virtual ~Dictionary() throw(); //!< destructor

            //! register the pattern
            const Pattern &operator()(const string &name, Pattern *p);

            //! register the pattern
            inline
            const Pattern &operator()(const char   *name, Pattern *p)
            {
                const string _ = name; return (*this)(_,p);
            }

            //! with direct compilation
            const Pattern &operator()(const string &name, const string &rx);

            //! with direct compilation
            const Pattern & operator()(const char *name, const char *rx)
            {
                const string _(name), __(rx); return (*this)(_,__);
            }

            //! get and clone pattern
            Pattern * create(const string &name) const;

            //! get and clone pattern
            inline
            Pattern * create(const char *name) const
            {
                const string _ = name; return create(_);
            }

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

