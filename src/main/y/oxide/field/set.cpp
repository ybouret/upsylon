#include "y/oxide/field/set.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Oxide
    {
        Fields:: ~Fields() throw()
        {
        }

        Fields:: Fields() throw()
        {

        }

        const char Fields:: Fn[] = "Oxide::Fields";

        void Fields:: __enroll(const FieldPointer   &f,
                               const std::type_info &t,
                               const void           *p)
        {
            const FieldHandle h(f,t,p);
            if( !insert(h) )
            {
                throw exception("%s(Multiple '%s', requested type <%s>)", Fn, * h.key(), t.name());
            }
        }


        const FieldHandle & Fields:: __lookUp( const string       &id, const std::type_info &t) const
        {
            const FieldHandle *h =  search(id);
            if(!h) throw exception("%s(No '%s')",Fn,*id);

            if(h->ftype != t )
            {
                throw exception("%s(%s<%s> type mismatch <%s>)", Fn, *id, h->ftype.name(), t.name() );
            }
            return *h;
        }

        bool Fields:: owns( const FieldType &F ) const throw()
        {
            const FieldHandle *h = this->search(F.name);
            if(!h)
            {
                std::cerr << "No Field '" << F.name << "' in Fields" << std::endl;
                return false;
            }
            else
            {
                const FieldPointer &p = h->field;
                if( & *p != &F )
                {
                    std::cerr << "Mismatch address of Field '" << F.name << "' in Fields" << std::endl;
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }


        FieldType & Fields:: operator[](const string &id)
        {
            const FieldHandle *h = search(id);
            if(!h)
            {
                throw exception("%s( no ['%s'])", Fn, *id );
            }
            const FieldType &cf = *(h->field);
            return (FieldType &)cf;
        }

        FieldType      & Fields:: operator[](const char *id)
        {
            const string _(id);
            return (*this)[_];
        }
    }

}

#include "y/string/tokenizer.hpp"
#include "y/sort/unique.hpp"

namespace upsylon
{
    namespace Oxide
    {

        Fields:: Selection:: Selection(const size_t n) :
        SelectionType(n,as_capacity)
        {
        }

        Fields:: Selection:: ~Selection() throw()
        {
        }

        Fields::Selection:: Selection(const Selection &other) :
        SelectionType(other)
        {
        }

        Fields:: Selection & Fields:: Selection:: operator()( FieldType &ft )
        {
            const FieldPointer p( &ft );
            push_back(p);
            clean();
            return *this;
        }


        Fields::Selection & Fields:: Selection::operator()( Fields &F, const string &ids )
        {
            Selection      &self = *this;
            tokenizer<char> tkn(ids);
            while( tkn.next_with(Separator) )
            {
                const string id( tkn.token(), tkn.units() );
                self( F[id] );
            }
            clean();
            return *this;
        }

        void Fields::Selection:: clean() throw()
        {
            unique(*this);
        }

    }

}
