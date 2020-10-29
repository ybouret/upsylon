//! \file
#ifndef Y_PREFIX_STEM_INCLUDED
#define Y_PREFIX_STEM_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/object.hpp"

namespace upsylon {

    struct prefix_
    {
    public:
        static const char *byte_to_text(const uint8_t &) throw();

        template <typename T> static inline const char *code_to_text(const T &x) throw()
        {
            const union {
                T       _;
                uint8_t b[ sizeof(T) ];
            } alias = { x };
            clr();
            for(size_t i=0;i<sizeof(T);++i) cat( byte_to_text(alias.b[i]));
            return text;
        }

    private:
        static char text[];
        static void clr() throw();
        static void cat(const char *) throw();
    };

    class prefix_data
    {
    public:
        typedef unsigned_int<sizeof(void*)>::type flag_t;
        
        explicit prefix_data() throw();
        virtual ~prefix_data() throw();
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_data);

    };

    template <typename CODE>
    class prefix_node : public object, public prefix_data
    {
    public:
        typedef core::list_of_cpp<prefix_node> list_type;
        CODE         code;
        prefix_node *next;
        prefix_node *prev;
        prefix_node *parent;
        list_type    leaves;
        size_t       freq;
        size_t       deep;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_node);
    };

}

#endif

