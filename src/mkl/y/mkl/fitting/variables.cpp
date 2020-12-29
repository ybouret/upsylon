
#include "y/mkl/fitting/variables.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

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
                const size_t iv = v->get_index(); assert(iv>0);
                for(iterator i=begin();i!=end();++i)
                {
                    const variable &tmp = **i;
                    if(tmp.get_index()==iv) throw exception("%s multiple index #%u for '%s' and '%s'", fn, unsigned(iv), *(v->name), *(tmp.name) );
                }
                local_add(v);
            }

            void variables:: local_add(const shared_variable &v)
            {
                if(!insert(v)) throw exception("%s multiple name '%s'", fn, *(v->name));
            }

            const shared_variable & variables:: operator()(const string &id) const
            {
                const shared_variable *ppV = search(id);
                if(!ppV)
                {
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
                    res = max_of(res, (**i).get_index() );
                }
                return res;
            }
        }
    }
}
