//! \file

#ifndef Y_JIVE_PATTERN_LEADING_INCLUDED
#define Y_JIVE_PATTERN_LEADING_INCLUDED 1

#include "y/jive/types.hpp"

namespace upsylon {
 
    namespace Jive {
        
        enum OwnerShip
        {
            OwnedByPrev,
            OwnedByThis,
            OwnedByNext
        };
        
        class Interval : public Object
        {
        public:
            typedef core::list_of_cpp<Interval> List;
            
            Interval      *next;
            Interval      *prev;
            const uint8_t  lower;
            const uint8_t  upper;
            
            Interval(const uint8_t) throw();
            Interval(const uint8_t,const uint8_t) throw();
            virtual ~Interval() throw();
            
            friend std::ostream & operator<<( std::ostream &, const Interval &);
            
            bool       owns(const uint8_t)  const throw();
            OwnerShip  whose(const uint8_t) const throw();
            size_t     width()              const throw();
            
            static Interval *TryMerge(const Interval *lhs, const Interval *rhs);
            static void      Compact3(List &L, Interval *a, Interval *b, Interval *c);
            static bool      AreApart(const Interval *lhs, const Interval *rhs) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Interval);
        };
        
        class Leading
        {
        public:
            explicit Leading() throw();
            virtual ~Leading() throw();
            Leading(const Leading &);
            
            friend std::ostream & operator<<( std::ostream &, const Leading &);

            bool   insert(const uint8_t c);
            bool   remove(const uint8_t c);
            size_t insert(const uint8_t lo,const uint8_t hi);
            void   release() throw();
            void   complete();
            
            const size_t size;
            
            
        private:
            Interval::List parts;
            bool check() const;
            bool removeFrom(Interval *node, const uint8_t c);
        };
        
        
    }
    
}

#endif

