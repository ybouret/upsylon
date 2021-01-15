
#include "y/mkl/fitting/variables.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            char variables::separator = ':';

            variables:: ~variables() throw()
            {

            }

            variables:: variables() : variables_()
            {
            }

            variables:: variables(const variables &other) :
            collection(), variables_(other)
            {
            }

            template <typename ID> static inline
            void __parse_variables(variables &self, const ID &names)
            {
                tokenizer<char>  t(names);
                while( t.next_with(variables::separator) )
                {
                    const string id( t.token(), t.units() );
                    self << id;
                }
            }

            variables:: variables(const string &names) : variables_()
            {
                __parse_variables(*this,names);
            }

            variables:: variables(const char *names) : variables_()
            {
                __parse_variables(*this,names);
            }


            variables & variables:: operator=(const variables &other)
            {
                {
                    variables_      & self = *this;
                    const variables & that = other;
                    self = that;
                }
                return *this;
            }

            static const char fn[] = "fitting::variables: ";

            void variables:: check_add(const shared_variable &v)
            {
                const size_t iv = **v; assert(iv>0);
                for(iterator i=begin();i!=end();++i)
                {
                    const variable &tmp = **i;
                    if(*tmp==iv) throw exception("%smultiple index #%u for '%s' and '%s'", fn, unsigned(iv), *(v->name), *(tmp.name) );
                }
                local_add(v);
            }

            void variables:: local_add(const shared_variable &v)
            {
                if(!insert(v)) throw exception("%smultiple name '%s'", fn, *(v->name));
            }

            const shared_variable & variables:: operator()(const string &id) const
            {
                const shared_variable *ppV = search(id);
                if(!ppV)
                {
                    throw exception("%sunknown variable '%s' among #%u",fn,*id, unsigned( size() ));
                }
                return *ppV;
            }

            const shared_variable & variables:: operator()(const char *id) const
            {
                const string _(id); return (*this)(_);
            }

            size_t variables:: max_name_length() const throw()
            {
                size_t res = 0;
                for(const_iterator i=begin();i!=end();++i)
                {
                    res = max_of(res, (**i).name.size() );
                }
                return res;
            }

            size_t variables:: sweep() const throw()
            {
                size_t res = 0;
                for(const_iterator i=begin();i!=end();++i)
                {
                    res = max_of(res,***i);
                }
                return res;
            }

            bool variables:: has(const string &id) const throw()
            {
                return 0 != search(id);
            }

            bool variables:: has(const char *id) const
            {
                const string _(id);
                return has(_);
            }

            size_t variables:: used_in(const accessible<bool> &used) const
            {
                size_t ans = 0;
                for(const_iterator it=begin();it!=end();++it)
                {
                    if( (**it)(used) ) ++ans;
                }
                return ans;
            }

        }
    }
}
