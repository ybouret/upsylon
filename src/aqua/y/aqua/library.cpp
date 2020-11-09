
#include "y/aqua/library.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include "y/exception.hpp"
#include "y/ios/align.hpp"
#include <iomanip>

namespace upsylon {

    namespace Aqua
    {
        Library:: ~Library() throw()
        {
        }

        Library:: Library() :
        suffix_storage<Species::Pointer>(),
        maxNameSize(0)
        {
        }


        Species & Library:: create(const Species::Pointer &sp)
        {
            if(!insert(sp->name,sp))
            {
                throw exception("Aqua::Library: multiple species '%s'", *(sp->name) );
            }
            aliasing::_(maxNameSize) = max_of(maxNameSize,sp->name.size());
            return aliasing::_(*sp);
        }

        std::ostream & Library:: display(std::ostream &os) const
        {
            os << '{' << std::endl;
            for(const_iterator it=begin();it!=end();++it)
            {
                os << '\t' << '{' << ' ';
                const Species &sp = **it;
                os << ios::align(sp.name,ios::align::left,maxNameSize);
                os << ' ' << ':' << ' ';
                os << std::setw(3) << sp.Z;
                os << ' ' << '}';
                os << ' ' << '@' << sp.indx;
                os << std::endl;
            }
            os << '}';
            return os;
        }

        std::ostream & operator<<(std::ostream &os, const Library &lib)
        {
            return lib.display(os);
        }

        void Library:: init() throw()
        {
            size_t i=0;
            for(iterator it=begin();it!=end();++it)
            {
                aliasing::_( (**it).indx ) = ++i;
            }
        }

        void Library:: show(std::ostream &os, const accessible<double> &C) const
        {
            os << '{' << std::endl;
            for(const_iterator it=begin();it!=end();++it)
            {
                const Species &sp = **it;
                assert(sp.indx>0);
                assert(sp.indx<=C.size());
                os << ' ' << '[' << ios::align(sp.name,ios::align::left,maxNameSize) << ']' << ' ' << '=' << ' ';
                os << C[sp.indx] << std::endl;
            }
            os << '}' << std::endl;
        }

        static const char fmt[] ="no Aqua::Libray[\"%s\"]";
        
        const Species & Library::  operator[](const string &id) const
        {
            const Species::Pointer *pps = search(id);
            if(!pps)
            {
                throw exception(fmt,*id);
            }
            return **pps;
        }

        const Species & Library:: operator[](const char *id) const
        {
            const Species::Pointer *pps = search(id);
            if(!pps)
            {
                throw exception(fmt,id);
            }
            return **pps;
        }




    }
}

