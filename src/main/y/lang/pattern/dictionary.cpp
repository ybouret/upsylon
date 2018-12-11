#include "y/lang/pattern/compiler.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        Dictionary:: Dictionary() throw() : CountedObject(), entries(), patterns()
        {
        }

        Dictionary:: ~Dictionary() throw()
        {
        }

        const Pattern & Dictionary:: operator()(const string &name, Pattern *p)
        {
            assert(p);
            //std::cerr << "Inserting Pattern as '" << name << "'" << std::endl;
            patterns.push_back(p);
            try
            {
                if( !entries.insert(name,p) )
                {
                    throw exception("Lang::Dictionary(multiple '%s')", *name);
                }
            }
            catch(...)
            {
                delete patterns.pop_back();
                throw;
            }
            return *p;
        }

        void Dictionary:: release() throw()
        {
            entries.release();
            patterns.clear();
        }

        Pattern * Dictionary:: create(const string &name) const
        {
            const Handle *ppP = entries.search(name);
            if(!ppP)
            {
                throw exception("Lang::Dictionary: no '%s'",*name);
            }
            const Pattern &p = **ppP;
            return p.clone();
        }


        const Pattern & Dictionary:: operator()(const string &name, const string &rx)
        {
            return (*this)(name, RegExp(rx,this));
        }


    }
}
