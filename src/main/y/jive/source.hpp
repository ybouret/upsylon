
//! \file

#ifndef Y_JIVE_SOURCE_INCLUDED
#define Y_JIVE_SOURCE_INCLUDED 1

#include "y/jive/module.hpp"
#include "y/core/nodes.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! smart, buffering pointer for a module
        //
        //______________________________________________________________________
        class Source : public CountedObject
        {
        public:
            explicit Source(Module *) throw();
            virtual ~Source() throw();

            void push(Module *);

            Char  *get();
            void   unget(Char *) throw();
            void   unget(Char::List &) throw();
            void   uncpy(const Char::List &);
            size_t depth() const throw();
            size_t in_cache() const throw();
            bool   alive();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Source);
            typedef core::singly<Module::Handle>  MetaModule;
            typedef core::pool_of_cpp<MetaModule> History;

            Module::Handle  handle;
            Char::List      cache;
            History         history;
            Char           *probe();
        };

    }

}

#endif
