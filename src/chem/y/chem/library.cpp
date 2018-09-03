
#include "y/chem/library.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Library:: ~Library() throw() {}

        Library:: Library(size_t n) : Object(), LibraryType(n,as_capacity)
        {
        }

        Library:: Library(const Library &other) : dynamic(), Object(), LibraryType(other)
        {
        }

        Species & Library:: operator()(const string &name, const int z)
        {
            Species::Pointer p = new Species(name,z,size()+1);
            if( !insert(p) )
            {
                throw exception("Chemical::Library(multiple '%s')", *name);
            }
            update();
            return *p;
        }

        void Library:: update() throw()
        {
            size_t indx = 0;
            for(iterator i=begin();i!=end();++i)
            {
                Species &sp = (**i);
                (size_t &)(sp.indx) = ++indx;
            }
        }


        size_t Library:: max_name_length() const throw()
        {
            size_t ans = 0;
            for(const_iterator i=begin();i!=end();++i)
            {
                const size_t tmp = (**i).name.size();
                if(tmp>ans) ans=tmp;
            }
            return ans;
        }

        Species & Library:: operator[](const string &id) const
        {
            const Species::Pointer *pp = search(id);
            if(!pp) throw exception("no species '%s'",*id);
            Species::Pointer q = *pp;
            return *q;
        }

    }
}
