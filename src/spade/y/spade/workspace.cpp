
#include "y/spade/workspace.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Spade {

        namespace Kernel
        {
            static const char fn[] = "Spade::Workspace: ";
            
            Workspace:: ~Workspace() throw()
            {
            }

            Workspace:: Workspace(const unsigned d) throw() :
            dimensions(d),
            fdb(),
            fields()
            {
            }
            
            
            void Workspace:: add(const _Field &P, const FieldClass cls )
            {
                const string &id = P->name;
                // into database
                if(! aliasing::_(fdb).insert(P))
                {
                    throw exception("%sadd multiple field '%s'", fn,*id);
                }

                // put it in fields if necessary
                switch (cls) {
                    case AsyncField:
                        try { aliasing::_(fields).push_back(P);      }
                        catch(...) { aliasing::_(fdb).no(id); throw; }
                        break;
                    case LocalField: break;
                }
            }

            bool Workspace:: owns(const Field &F) const throw()
            {
                const _Field *p = fdb.search(F.name);
                return( NULL!=p) && ( & **p == & F) ;
            }

            bool Workspace:: owns(const _Field &F) const throw()
            {
                const _Field *p = fdb.search(F->name);
                return( NULL!=p) && (*p == F) ;
            }

            bool Workspace:: ownsAll(const accessible<_Field> &F) const throw()
            {
                for(size_t i=F.size();i>0;--i)
                {
                    if( ! owns(F[i]) ) return false;
                }
                return true;
            }

            const Field & Workspace:: getField(const string &id) const
            {
                const _Field *p =  fdb.search(id);
                if(!p) throw exception("%sno const ['%s']",fn,*id);
                return **p;
            }
            
            const Field & Workspace:: getField(const char *id) const
            {
                const string _(id);
                return getField(_);
            }
            
            size_t Workspace:: CheckRank(const size_t size, const size_t rank)
            {
                if(rank>=size) throw exception("%srank=%lu>=size=%lu",fn,(unsigned long)size, (unsigned long)rank );
                return rank;
            }
            
            void Workspace:: CheckMappingValue(const size_t size, const Coord1D prod)
            {
                if( size_t(prod) != size ) throw exception("%sinvalid mapping for size=%lu", fn,(unsigned long)size);
            }
            
            
        }

        
    }
    
}

#include "y/sort/heap.hpp"

namespace upsylon {
    
    namespace Spade {
        
        FieldsIO:: FieldsIO() throw() : FieldsIO_() {}
        
        FieldsIO:: FieldsIO(const size_t n) : FieldsIO_(n,as_capacity) {}

        
        FieldsIO:: ~FieldsIO() throw() {}
        
        FieldsIO:: FieldsIO(const FieldsIO &other) :
        collection(), addressable<_Field>(), counted_object(), FieldsIO_(other) {}
        
        FieldsIO & FieldsIO:: operator=( const FieldsIO &other )
        {
            FieldsIO_ &self = *this;
            self = other;
            return *this;
        }
        
        static inline int compareByName(const _Field &lhs, const _Field &rhs ) throw()
        {
            return string::compare(lhs->name, rhs->name);
        }
        
        void FieldsIO:: sort() throw()
        {
            hsort(*this,compareByName);
        }

        FieldsIO & FieldsIO:: operator<<( Field &f )
        {
            if(f.refcount()<=0) throw exception("Spade::FieldsIO(field '%s' is not dynamic)", *f.name);
            const _Field F = &f;
            push_back(F);
            return *this;
        }

        
    }
}

