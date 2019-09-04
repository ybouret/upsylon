//! \file
#ifndef Y_COUNT_DANCING_INCLUDED
#define Y_COUNT_DANCING_INCLUDED 1

#include "y/core/list.hpp"
#include "y/object.hpp"

namespace upsylon
{
    class dancing
    {
    public:
        class guest : public object
        {
        public:
            const size_t label;
            guest       *next;
            guest       *prev;

            explicit guest(const size_t i) throw() :
            object(), label(i), next(0), prev(0) {}
            virtual ~guest() throw() {}

            typedef core::list_of_cpp<guest> list_type;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(guest);
        };

        class group : public object, public guest::list_type
        {
        public:
            group *next;
            group *prev;

            explicit group() throw() : object(), guest::list_type(), next(0), prev(0)
            {
            }

            virtual ~group() throw()
            {
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(group);
        };



        explicit dancing(const size_t n,
                         const size_t k);

        virtual ~dancing() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dancing);
    };
}

#endif

