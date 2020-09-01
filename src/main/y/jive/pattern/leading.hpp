//! \file

#ifndef Y_JIVE_PATTERN_LEADING_INCLUDED
#define Y_JIVE_PATTERN_LEADING_INCLUDED 1

#include "y/jive/types.hpp"

namespace upsylon {
 
    namespace Jive {

        //______________________________________________________________________
        //
        //! testing ownership
        //______________________________________________________________________
        enum OwnerShip
        {
            OwnedByPrev, //!< in prev interval
            OwnedByThis, //!< got it
            OwnedByNext  //!< in next interval
        };

        //______________________________________________________________________
        //
        //! interval of bytes
        //______________________________________________________________________
        class Interval : public Object
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef core::list_of_cpp<Interval> List; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            Interval(const uint8_t) throw();                 //!< setup
            Interval(const uint8_t,const uint8_t) throw();   //!< setup
            virtual ~Interval() throw();                     //!< cleanup
            friend std::ostream & operator<<(std::ostream &, const Interval &); //!< display

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool             owns(const uint8_t)  const throw();                         //!< check insidie
            OwnerShip        whose(const uint8_t) const throw();                         //!< hint for owner
            size_t           width()              const throw();                         //!< interval widht
            static Interval *TryMerge(const Interval *, const Interval *);               //!< possible merge
            static void      Compact3(List &L, Interval *a, Interval *b, Interval *c);   //!< all posible merges
            static bool      AreApart(const Interval *lhs, const Interval *rhs) throw(); //!< to debug

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Interval      *next;
            Interval      *prev;
            const uint8_t  lower;
            const uint8_t  upper;

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

