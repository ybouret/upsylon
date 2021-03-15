
//! \file

#ifndef Y_GFX_FILTERS_INCLUDED
#define Y_GFX_FILTERS_INCLUDED 1

#include "y/gfx/filter.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        typedef filter<float>              filter_type;
        typedef arc_ptr<const filter_type> shared_filter;
        
        class filters : public entity
        {
        public:
            virtual ~filters() throw();
            
            const string        name;
            const shared_filter y;
            const shared_filter x;
            
        protected:
            template <typename ID, typename U>
            inline explicit filters(const ID    &ident,
                                    const U     *coeff,
                                    const unit_t width) :
            name(ident),
            y( new filter_type(name,coeff,width,false, "_y") ),
            x( new filter_type(name,coeff,width,true,  "_x") )
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(filters);
        };
        
        typedef arc_ptr<const filters> shared_filters;
        
    }
    
}

#endif

