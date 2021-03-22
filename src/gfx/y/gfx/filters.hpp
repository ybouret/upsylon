
//! \file

#ifndef Y_GFX_FILTERS_INCLUDED
#define Y_GFX_FILTERS_INCLUDED 1

#include "y/gfx/filter.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        
        
        typedef filter<float,int>                  filter_type;   //!< built-in filter type
        typedef intr_ptr<string,const filter_type> shared_filter; //!< allocated filter
        
        //______________________________________________________________________
        //
        //
        //! named filters for Y and X
        //
        //______________________________________________________________________
        class filters : public entity
        {
        public:
            virtual ~filters() throw(); //!< cleanup
            
            const string        name;   //!< identifier
            const shared_filter y;      //!< y filter
            const shared_filter x;      //!< x filter (y-transposed)
            
            const string &key() const throw(); //!< name
            
        protected:
            //! setup filters from a table
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
        
        //______________________________________________________________________
        //
        //
        //! allocated filters
        //
        //______________________________________________________________________

        typedef intr_ptr<string,const filters> shared_filters;
        
    }
    
}
//______________________________________________________________________


//! declare a filter NAME/DIM
#define Y_FILTERS_DECL(NAME,DIM)           \
/**/ class NAME##DIM : public filters {    \
/**/ public:                               \
/**/ static const char ID[];               \
/**/ explicit NAME##DIM();                 \
/**/ virtual ~NAME##DIM() throw();         \
/**/ private:                              \
/**/ Y_DISABLE_COPY_AND_ASSIGN(NAME##DIM); \
/**/ static const int Coeffs[DIM][DIM];    \
/**/ }

//! implement a filter NAME/DIM
#define Y_FILTERS_IMPL(NAME,DIM)                                 \
/**/ const char   NAME##DIM :: ID[] = #NAME #DIM;                \
/**/ NAME##DIM::  NAME##DIM() : filters(ID,&Coeffs[0][0],DIM) {} \
/**/ NAME##DIM:: ~NAME##DIM() throw() {}                         \
/**/ const int NAME##DIM :: Coeffs[DIM][DIM] = {

//! done implementing coefficients...
#define Y_FILTERS_DONE() }

#endif

