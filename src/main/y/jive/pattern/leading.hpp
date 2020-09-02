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
            Interval(const Interval &) throw();              //!< copy
            ~Interval() throw();                             //!< cleanup
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
            Interval      *next;  //!< for list
            Interval      *prev;  //!< for list
            const uint8_t  lower; //!< lower bound
            const uint8_t  upper; //!< upper bound

        private:
            Y_DISABLE_ASSIGN(Interval);
        };

        //______________________________________________________________________
        //
        //! possible leading bytes
        //______________________________________________________________________
        class Leading
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Leading() throw(); //!< setup empty
            virtual ~Leading() throw(); //!< cleanup
            Leading(const Leading &);   //!< copy

            //! display
            friend std::ostream & operator<<( std::ostream &, const Leading &);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool   insert(const uint8_t c);             //!< insert a byte, true if new
            bool   remove(const uint8_t c);             //!< remove a byte, true if found
            size_t insert(const uint8_t,const uint8_t); //!< insert a range, return number of new bytes
            void   release() throw();                   //!< release all
            void   complete();                          //!< set full interval
            void   opposite(const Leading &);           //!< complete and exclude
            void   include(const Leading &);            //!< include another
            void   exclude(const Leading &);            //!< exclude another
            void   commute(Leading &) throw();          //!< no-throw exchange
            bool   search(const uint8_t) const throw(); //!< search for owned
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t size; //!< 0..256 leading bytes

        private:
            Interval::List parts;
            bool check() const;
            bool removeFrom(Interval *node, const uint8_t c);
        };
        
        
    }
    
}

#endif

