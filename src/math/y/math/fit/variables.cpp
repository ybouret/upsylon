#include "y/math/fit/variables.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"





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
                const size_t nv = tokenizer<char>::count_for(ids, is_colon);
                free();
                ensure(nv);
                tokenizer<char> tkn(ids);
                while( tkn.next(is_colon) )
                {
                    const string id( tkn.token(), tkn.units() );
                    (*this) << id;
                }
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

            Variables:: Variables(const Variables &other) : collection(), Variable::Set(other) {}

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


