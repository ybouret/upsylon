#include "y/lang/pattern/dictionary.hpp"
#include "y/lang/pattern/compiler.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        Dictionary:: Dictionary() throw() : CountedObject(), Dict::Patterns()
        {
        }


        Dictionary:: Dictionary(const size_t n) : CountedObject(), Dict::Patterns(n,as_capacity)
        {
        }

        Dictionary:: ~Dictionary() throw()
        {
        }

        const Pattern & Dictionary:: operator()(const string &name, Pattern *p)
        {
            assert(p);
            const Dict::Motif m = p;
            if( !insert(name,m) )
            {
                throw exception("Lang::Dictionary(multiple '%s')", *name);
            }
            return *p;
        }

        const Pattern & Dictionary:: operator()(const char   *name, Pattern *p)
        {
            const string _ = name; return (*this)(_,p);
        }

        const Pattern & Dictionary:: operator[](const string &name) const
        {
            const Dict::Motif  *pp = search(name);
            if(!pp)
            {
                throw exception("Lang::Dictionary: no '%s'",*name);
            }
            return **pp;
        }

        const Pattern & Dictionary:: operator[](const char *name) const
        {
            const Dictionary &self = *this;
            const string      _(name);
            return self[_];
        }

        Pattern * Dictionary:: create(const string &name) const
        {
            const Dictionary &self = *this;
            return self[name].clone();
        }

        Pattern * Dictionary:: create(const char *name) const
        {
            const string _ = name; return create(_);
        }

        const Pattern & Dictionary:: operator()(const string &name, const string &rx)
        {
            return (*this)(name, RegExp(rx,this));
        }

        const Pattern & Dictionary::  operator()(const char *name, const char *rx)
        {
            const string _(name), __(rx); return (*this)(_,__);
        }


    }
}
