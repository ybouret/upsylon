//! \file

#ifndef Y_JIVE_CHAR_INCLUDED
#define Y_JIVE_CHAR_INCLUDED 1

#include "y/jive/context.hpp"
#include "y/memory/tight/supply.hpp"

namespace upsylon {

    namespace Jive
    {
        
        //______________________________________________________________________
        //
        //
        //! a fully qualified char
        //
        //______________________________________________________________________
        class Char : public inode<Char>, public Context
        {
        public:

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static Char *Acquire(const Context &, const uint8_t);           //!< create
            static void  Release(Char *) throw();                           //!< delete
            static Char *Copycat(const Char &);                             //!< copy
            static Char *Copyset(const Char &, const uint8_t);              //!< copy with another byte
            static void  Reserve(const size_t);                             //!< get memory from system
            friend std::ostream & operator<<(std::ostream &, const Char &); //!< display
            friend ios::ostream & operator<<(ios::ostream &, const Char &); //!< append to ostream

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint8_t code; //!< the code

        private:
            Y_DISABLE_ASSIGN(Char);
            explicit Char(const Context &, const uint8_t code) throw();
            explicit Char(const Char &) throw();
            virtual ~Char() throw();

        public:
            typedef memory::tight::supply_of<Char> SupplyType;

            //__________________________________________________________________
            //
            //! thread safe specialized Char::Supply
            //__________________________________________________________________
            class Supply : public singleton<Supply>, public SupplyType
            {
            public:

                Char *acquire(const Context &context,
                              const uint8_t  code); //!< zeroed memory for Char
                void  release(Char *ch) throw();    //!< return memory
                void  reserve(size_t);              //!< query from system
                Char *copycat(const Char&);         //!< copy

            private:
                Y_SINGLETON_DECL(Supply);
                Y_DISABLE_COPY_AND_ASSIGN(Supply);
                explicit Supply();
                virtual ~Supply() throw();
            };

            //__________________________________________________________________
            //
            //! base class for Char::List
            //__________________________________________________________________
            typedef core::list_of<Char> ListType;

            //__________________________________________________________________
            //
            //! list of Chars
            //__________________________________________________________________
            class List : public ListType, public releasable
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                explicit List() throw();        //!< setup empty
                virtual ~List() throw();        //!< cleanup
                List( const List &);            //!< copy
                List & operator=(const List &); //!< copy/swap

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                List & operator<<(Char *) throw();         //!< helper
                List & add(const Context &,const uint8_t); //!< helper
                List & operator<<(const List &);           //!< helper
                virtual void release() throw();            //!< erase!
                const Char & head_char() const throw();    //!< *head for size>0
                const Char & tail_char() const throw();    //!< *tail for size>0
                
                //! display
                friend std::ostream & operator<<(std::ostream &, const List &);

                //! append to ios
                friend ios::ostream & operator<<(ios::ostream &, const List &);

            };



        };


    }

}
#endif

