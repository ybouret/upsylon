
//! \file

#ifndef Y_IOS_CONVEYOR_LIMITED_INCLUDED
#define Y_IOS_CONVEYOR_LIMITED_INCLUDED 1

#include "y/ios/conveyor.hpp"
#include "y/type/args.hpp"

namespace upsylon {
    
    namespace ios
    {
        template <typename CONVEYOR>
        class limited_conveyor_
        {
        public:
            const CONVEYOR io;
            
            inline explicit limited_conveyor_() throw() : io() {}
            inline virtual ~limited_conveyor_() throw()        {}
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(limited_conveyor_);
        };
        
        template <typename T,
        template <class> class CONVEYOR>
        class limited_conveyor : public limited_conveyor_< CONVEYOR<T> >, public conveyor
        {
        public:
            typedef limited_conveyor_< CONVEYOR<T> > base_type;
            const size_t      blocks;
            
            inline virtual ~limited_conveyor() throw()
            {
            }
            
            inline explicit limited_conveyor(const size_t n, const std::type_info &_) throw() :
            base_type(),
            conveyor(this->io.comms_manner,_),
            blocks(n)
            {
            }
            
            inline virtual void copy(void *target, const void *source) const
            {
                char       *p = static_cast<char *>(target);
                const char *q = static_cast<const char *>(source);
                for(size_t i=blocks;i>0;--i)
                {
                    this->io.copy(p,q);
                    p += sizeof(T);
                    q += sizeof(T);
                }
            }
            
            inline virtual size_t save( ios::ostream &target, const void *source ) const
            {
                const char *q = static_cast<const char *>(source);
                size_t      total = 0;
                for(size_t i=blocks;i>0;--i)
                {
                    total += this->io.save(target,q);
                    q += sizeof(T);
                }
                return total;
            }
            
            inline virtual size_t load(void *target, ios::istream &source) const
            {
                char  *p     = static_cast<char *>(target);
                size_t total = 0;
                for(size_t i=blocks;i>0;--i)
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
