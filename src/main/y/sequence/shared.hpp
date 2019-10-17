//! \file

#ifndef Y_SHARED_SEQUENCE_INCLUDED
#define Y_SHARED_SEQUENCE_INCLUDED 1

#include "y/ptr/counted.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon {


    template <typename SEQUENCE>
    class tableau  : public counted_object, public SEQUENCE
    {
    public:
        typedef typename SEQUENCE::type         type;
        typedef typename SEQUENCE::mutable_type mutable_type;
        typedef typename SEQUENCE::const_type   const_type;

        inline virtual ~tableau() throw() {}
        inline explicit tableau() : SEQUENCE() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(tableau);

    };


#if 0
    template <typename T>
    class tableau : public counted_object, public addressable<T>
    {
    public:
        Y_DECL_ARGS(T,type);
        inline virtual ~tableau() throw() {}

    protected:
        inline explicit tableau() throw() : counted_object(), addressable<T>() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(tableau);
    };



    template <typename SEQUENCE>
    class tableau_for : public tableau< typename SEQUENCE::type >
    {
    public:
        typedef typename SEQUENCE::type         type;
        typedef typename SEQUENCE::mutable_type mutable_type;
        typedef typename SEQUENCE::const_type   const_type;
        typedef tableau<type>                   tableau_type;

        SEQUENCE content;

        inline virtual ~tableau_for() throw() {}

        inline          tableau_for() : tableau_type(), content() {}

        inline          tableau_for(const size_t n, const as_capacity_t &_) :
        tableau_type(), content(n,_) {}

        inline          tableau_for(const tableau_for &other) :
        tableau_type(),content(other) {}


        inline SEQUENCE       & operator*() throw()       { return content; }
        inline const SEQUENCE & operator*() const throw() { return content; }



    private:
        Y_DISABLE_ASSIGN(tableau_for);

    };
#endif

}

#endif
