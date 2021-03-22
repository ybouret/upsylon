#include "y/gfx/filters/db.hpp"
#include "y/exception.hpp"

#include "sobel.hpp"
#include "scharr.hpp"
#include "prewitt.hpp"


namespace upsylon
{
    Y_SINGLETON_IMPL(graphic::filters_db);
    
    namespace graphic
    {
        static const char fn[] = "graphic::filters_db: ";
        
        filters_db:: ~filters_db() throw()
        {
        }
     
        filters_db:: filters_db() : filters_db_()
        {
            use( new Sobel3() );
            use( new Sobel5() );
            use( new Sobel7() );

            use( new Prewitt3() );
            use( new Prewitt5() );
            use( new Prewitt7() );
            
            use( new Scharr3() );
            use( new Scharr5() );

            
        }
        
        void filters_db:: use(filters *f)
        {
            assert(f);
            const shared_filters F = f;
            if(!insert(F))
            {
                throw exception("%smultiple '%s'", fn, *(F->name) );
            }
        }
        
        const shared_filters & filters_db:: operator[](const string &id) const
        {
            const shared_filters *ppF = search(id);
            if(!ppF) throw exception("%smissing '%s'",fn,*id);
            return *ppF;
        }

        const shared_filters & filters_db:: operator[](const char *id) const
        {
            const string _(id);
            return (*this)[_];
        }
        
    }
    
}
