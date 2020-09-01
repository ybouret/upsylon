
//! \file

#ifndef Y_JIVE_SOURCE_INCLUDED
#define Y_JIVE_SOURCE_INCLUDED 1

#include "y/jive/module.hpp"
#include "y/core/nodes.hpp"

namespace upsylon {

    namespace Jive
    {

        class Leading; //!< forward declaration

        //______________________________________________________________________
        //
        //
        //! smart, buffering pointer for a module
        //
        //______________________________________________________________________
        class Source : public CountedObject
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Source(Module *) throw(); //!< setup with default module
            virtual ~Source() throw();         //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void push(Module *); //!< push a new module for reading

            Char  *get();                      //!< get next char within cache/modules
            void   unget(Char *) throw();      //!< unget a char
            void   unget(Char::List &) throw();//!< unget a char list
            void   uncpy(const Char::List &);  //!< unget a copy of a char list
            size_t depth() const throw();      //!< sizeof history
            size_t in_cache() const throw();   //!< cache.size
            bool   alive();                    //!< found a readable char
            void   skip() throw();             //!< assert(cache.size>0);
            void   skip(size_t n) throw();     //!< assert(cache.size>=n);
            bool   find(const Leading &);      //!< skip until first char is found

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
