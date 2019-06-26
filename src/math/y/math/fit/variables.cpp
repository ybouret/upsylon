#include "y/math/fit/variables.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            Variable:: ~Variable() throw() {}
            
            Variable:: Variable(const string & __name, const Type __type) :
            name( __name ), type( __type ) {}

            const string & Variable:: key() const throw() { return name; }


            size_t Variable:: check_index( const size_t against_size ) const
            {
                const size_t indx = index();
                if(indx<=0||indx>against_size)
                {
                    throw exception("Invalid index %u/%u for Variable '%s'", unsigned(indx), unsigned(against_size), *name );
                }
                return indx;
            }
        }

    }
}

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            GlobalVariable:: GlobalVariable(const string & __name, const size_t __indx ) :
            Variable(__name,IsGlobal),
            indx(__indx)
            {
                if(indx<=0) throw exception("%s.indx<=0", *name );
            }

            GlobalVariable:: ~GlobalVariable() throw()
            {
            }

            size_t GlobalVariable:: index() const throw() { return indx; }

        }

    }
}

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            LocalVariable:: LocalVariable( const string & __name, const Variable::Pointer &__link ) :
            Variable( __name, IsLocal ),
            link(__link)
            {
            }

            LocalVariable:: ~LocalVariable() throw() {}
            
            size_t LocalVariable:: index() const throw() { return link->index(); }
        }
    }
}


namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            Variables:: Variables(const size_t n) : Variable::Set(n,as_capacity)
            {

            }


            void Variables::create_from_list(const string &ids)
            {
                
            }


            Variables:: Variables(const string &var_list) : Variable::Set()
            {
                create_from_list(var_list);
            }

            Variables:: Variables(const char *var_list) : Variable::Set()
            {
                const string _(var_list);
                create_from_list(_);
            }


            Variables:: ~Variables() throw() {}

            Variables:: Variables(const Variables &other) : dynamic(), object(), Variable::Set(other) {}

            Variables & Variables:: operator=( const Variables &other )
            {
                Variable::Set &self = *this;
                self = other;
                return *this;
            }

            Variables & Variables:: create_global(const string &name)
            {
                const Variable::Pointer p = new GlobalVariable(name,size()+1);
                if( ! insert(p) )
                {
                    throw exception("Variables( Multiple Global '%s' )", *name );
                }
                return *this;
            }

            Variables & Variables:: create_global(const char *name)
            {
                const string _(name); return create_global(_);
            }

            Variables & Variables:: operator<<( const string &name) { return create_global(name); }

            Variables & Variables:: operator<<( const char *name) { return create_global(name); }



            const Variable::Pointer & Variables:: operator[](const string &name) const
            {
                const Variable::Pointer *ppVar = search(name);
                if(!ppVar)
                {
                    throw exception("no ['%s'] amongst %u variable%s", *name, unsigned(size()), plural_s(size()));
                }
                return *ppVar;
            }

            const Variable::Pointer & Variables:: operator[](const char *name) const
            {
                const string _ = name; return (*this)[_];

            }

            Variables & Variables:: operator()(const string &name) { return create_global(name); }

            Variables & Variables:: operator()(const char   *name) { return create_global(name); }

            Variables & Variables:: operator()(const string &name, const Variable::Pointer &link)
            {
                const Variable::Pointer p = new LocalVariable(name,link);
                if(!insert(p))
                {
                    throw exception("Variables( Multiple Local '%s' )", *name );
                }
                return *this;
            }

            Variables        &  Variables:: operator()(const string &name, const Variables &global)
            {
                const Variable::Pointer &link = global[name];
                return (*this)(name,link);
            }

            Variables & Variables:: operator()(const string &prefix, Variables &global, const size_t index)
            {
                const string global_name = prefix + vformat( "_%lu", static_cast<unsigned long>(index) );
                global << global_name;
                return (*this)(prefix,global[global_name]);
            }

            Variables & Variables:: operator()(const char *prefix, Variables &global, const size_t index)
            {
                const string _(prefix);
                return (*this)(_,global,index);
            }

            size_t Variables:: get_max_name_size() const throw()
            {
                size_t sz = 0;
                for( const_iterator i=begin();i!=end();++i)
                {
                    const string &name = (**i).name;
                    sz = max_of(sz,name.size());
                }
                return sz;
            }

            Variables & Variables:: operator()(const char   *name, const Variable::Pointer &link)
            {
                const string _(name); return (*this)(_,link);
            }

            Variables & Variables:: operator()(const char   *name, const Variables &global)
            {
                const string _(name); return (*this)(_,global);
            }

        }

    }
}


