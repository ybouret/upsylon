//! \file

#ifndef Y_IOS_ALIGN_INCLUDED
#define Y_IOS_ALIGN_INCLUDED 1

#include "y/string.hpp"

namespace upsylon {

    namespace ios
    {
        //! ligth weight class to align strings
        class align
        {
        public:
            //! justification
            enum justify
            {
                left,   //!< left
                center, //!< center
                right   //!< right
            };

            const char     *text;  //!< persistent text
            const size_t    size;  //!< persistent size
            const justify   kind;  //!< kind of justification
            const size_t    width; //!< larger width

            ~align() throw();              //!< cleanup
            align(const align &) throw();  //!< no-throw copy
            align(const char   *, const size_t, const justify, const size_t) throw(); //!< setup
            align(const char   *, const justify, const size_t) throw();               //!< setup
            align(const string &, const justify, const size_t) throw();               //!< setup
            align(const char   &, const justify, const size_t) throw();               //!< setup

            //! output
            template <typename OSTREAM>
            friend inline OSTREAM & operator<<(OSTREAM &os, const align &a)
            {
                if(a.width>a.size)
                {
                    const size_t full = a.width-a.size;
                    switch(a.kind)
                    {
                        case left:
                            a.print(os); print(os,full);
                            break;

                        case center: {
                            const size_t half = full >> 1;
                            a.print(os,half); a.print(os); a.print(os,full-half);
                        } break;

                        case right:
                            print(os,full); a.print(os);
                            break;

                    }
                }
                else
                {
                    a.print(os);
                }
                return os;
            }


        private:
            Y_DISABLE_ASSIGN(align);
            template <typename OSTREAM> inline
            void print(OSTREAM &os) const
            {
                for(size_t i=0;i<size;++i)
                {
                    os << text[i];
                }
            }

            template <typename OSTREAM> static inline
            void print(OSTREAM &os, size_t n)
            {
                while(n-- > 0) os << ' ';
            }
        };



    }

}

#endif

