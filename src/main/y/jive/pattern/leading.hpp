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
        
        class Leading : public Object
        {
        public:
            static const  uint8_t  Byte = 0x01;
            static const  uint8_t  Tier = 0x02;
            static const  unsigned SHL  = 0x04;
            typedef core::list_of_cpp<Leading> List;
            
            Leading      *next;
            Leading      *prev;
            const uint8_t kind;
            const uint8_t code;
            const uint8_t lower;
            const uint8_t upper;
            
            Leading(const uint8_t) throw();
            Leading(const uint8_t,const uint8_t) throw();
            Leading(const Leading &) throw();
            virtual ~Leading() throw();
            
            friend std::ostream & operator<<( std::ostream &, const Leading &);
            
            bool       owns(const uint8_t)  const throw();
            OwnerShip  whose(const uint8_t) const throw();
            size_t     count()           const throw();
            
            static Leading *TryMerge(const Leading *lhs, const Leading *rhs);
            static void     Compact3(Leading::List &L, Leading *a, Leading *b, Leading *c);
            static bool     AreApart(const Leading *lhs, const Leading *rhs) throw();

        private:
            Y_DISABLE_ASSIGN(Leading);
        };
        
        class LeadingChars
        {
        public:
            explicit LeadingChars() throw();
            virtual ~LeadingChars() throw();
            LeadingChars( const LeadingChars &);
            
            friend std::ostream & operator<<( std::ostream &, const LeadingChars &);

            bool insert(const uint8_t c);
            bool remove(const uint8_t c);
            void release() throw();
            void complete();
            //void opposite();
            
            const size_t size;
            
            
        private:
            Leading::List lead;
            bool check() const;
            bool removeFrom(Leading *node, const uint8_t c);
        };
        
        
    }
    
}

#endif

