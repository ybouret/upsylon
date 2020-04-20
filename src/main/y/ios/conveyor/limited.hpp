
//! \file

#ifndef Y_IOS_CONVEYOR_LIMITED_INCLUDED
#define Y_IOS_CONVEYOR_LIMITED_INCLUDED 1

#include "y/ios/conveyor.hpp"
#include "y/type/args.hpp"

namespace upsylon {
    
    namespace ios
    {
        //----------------------------------------------------------------------
        //
        //! build internal conveyor
        //
        //----------------------------------------------------------------------
        template <typename CONVEYOR>
        class limited_conveyor_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit limited_conveyor_() throw() : io() {} //!< setup
            inline virtual ~limited_conveyor_() throw()        {} //!< cleanup
        
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const CONVEYOR io; //!< the internal conveyor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(limited_conveyor_);
        };
        
        //----------------------------------------------------------------------
        //
        //! aggregate a multiple CONVEYOR
        //
        //----------------------------------------------------------------------
        template <
        const size_t           BLOCKS,
        typename               T,
        template <class> class CONVEYOR
        >
        class limited_conveyor : public limited_conveyor_< CONVEYOR<T> >, public conveyor
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef limited_conveyor_< CONVEYOR<T> > base_type; //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
           
            //! cleanup
            inline virtual ~limited_conveyor() throw()
            {
            }
            
            //! setup
            inline explicit limited_conveyor(const std::type_info &_) throw() :
            base_type(),
            conveyor(this->io.mode,_,BLOCKS*sizeof(T))
            {
            }
            
            //__________________________________________________________________
            //
            // conveyor interface
            //__________________________________________________________________
            
            //! copy BLOCKS types
            inline virtual void copy(void *target, const void *source) const
            {
                char       *p = static_cast<char *>(target);
                const char *q = static_cast<const char *>(source);
                for(size_t i=BLOCKS;i>0;--i)
                {
                    this->io.copy(p,q);
                    p += sizeof(T);
                    q += sizeof(T);
                }
            }
            
            //! save BLOCKS types
            inline virtual size_t save( ios::ostream &target, const void *source ) const
            {
                const char *q     = static_cast<const char *>(source);
                size_t      total = 0;
                for(size_t i=BLOCKS;i>0;--i)
                {
                    total += this->io.save(target,q);
                    q += sizeof(T);
                }
                return total;
            }
            
            //! load BLOCKS types
            inline virtual size_t load(void *target, ios::istream &source) const
            {
                char  *p     = static_cast<char *>(target);
                size_t total = 0;
                for(size_t i=BLOCKS;i>0;--i)
                {
                    total += this->io.load(p,source);
                    p += sizeof(T);
                }
                return total;
            }
            
                
        private:
            Y_DISABLE_COPY_AND_ASSIGN(limited_conveyor);
        };
    }
    
}

#endif
