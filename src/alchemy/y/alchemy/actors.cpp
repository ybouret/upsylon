#include "y/alchemy/actors.hpp"
#include "y/exception.hpp"

#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include "y/code/decimal-chars.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace upsylon
{
    namespace Alchemy
    {

#if 0
        const char actors:: clid[] = "alchemy::actors";
        
        actors:: actors() :
        size(0),
        indx(0),
        coef(0),
        db(),
        wlen(0)
        {
        }
        
        actors:: ~actors() throw()
        {
            if(wlen)
            {
                static memory::allocator &mgr = memory::dyadic::location();
                mgr.release( *(void **)&(++aliasing::_(indx)), wlen);
                aliasing::_(coef) = 0;
                aliasing::_(size) = 0;
            }
        }
        
        
        void actors:: operator()(const species &sp, const unsigned long nu)
        {
            assert(!compiled);
            if(nu<=0)    throw exception("%s(nul coefficient for '%s')", clid, *sp.name);
            const actor a(sp,nu);
            if( !db.insert(a) ) throw exception("%s(multiple '%s')", clid, *sp.name);
        }
        
        bool actors:: search(const string &id) const throw()
        {
            return NULL != db.search(id);
        }
        
        const actor::db & actors::bulk()  const throw()
        {
            return db;
        }
        
        
        
        void actors:: on_compile()
        {
            assert(!compiled);
            static memory::allocator &mgr = memory::dyadic::instance();
            
            const size_t n = db.size();
            size_t       w = 0;
            
            for(type::iterator it=db.begin();it!=db.end();++it)
            {
                const species &sp = **it;
                if(sp.indx<=0) throw exception("%s: index of '%s' is 0",clid, *sp.name);
            }
            
            if(n>0)
            {
                wlen = 2*n * sizeof(size_t);
                aliasing::_(indx) = static_cast<size_t*>( mgr.acquire(wlen) )-1;
                aliasing::_(coef) = indx + n;
                {
                    size_t i=1;
                    for(type::iterator it=db.begin();i<=n;++it,++i)
                    {
                        const actor &a = *it;
                        aliasing::_(indx[i]) = a->indx;
                        aliasing::_(coef[i]) = a.nu;
                        w = max_of(w, decimal_chars_for(a.nu) );
                    }
                }
                
                for(type::iterator it=db.begin(); it!=db.end(); ++it)
                {
                    aliasing::_( (*it).cw ) = w;
                }
                
                aliasing::_(size) = n;
                
            }
        }
        
        
        extent actors:: find_extent(const accessible<double> &C) const throw()
        {
            assert(compiled);
            double        xm   = 0;
            size_t        im   = 0;
            size_t        ii  = size;
            if(ii)
            {
                const size_t *idx = indx;
                const size_t *cof = coef;
                
                // initialize
                im = idx[ii];       assert(im>0); assert(im<=C.size());
                xm = C[im]/cof[ii];
                
                // scan
                for(--ii;ii>0;--ii)
                {
                    const size_t it = idx[ii]; assert(it>0); assert(it<=C.size());
                    const double xt = C[ it ]/cof[ii];
                    if(xt<xm)
                    {
                        xm = xt;
                        im = it;
                    }
                }
            }
            
            return extent(im,xm);
        }
#endif
        
    }
    
}

