//! \file
#ifndef Y_STRING_BASIC_INCLUDED
#define Y_STRING_BASIC_INCLUDED 1

#include "y/memory/pooled.hpp"
#include "y/memory/buffer.hpp"
#include "y/type/cswap.hpp"
#include "y/dynamic.hpp"
#include "y/comparison.hpp"
#include <cstring>
#include <iosfwd>

namespace upsylon
{
    //! secured string length
    template <typename T>
    size_t length_of( const T *s ) throw()
    {
        if(!s)
        {
            return 0;
        }
        else
        {
            const T *start = s;
            while( *s ) { ++s; }
            return s-start;
        }
    }

    //! sanity check of a core string
#define Y_CORE_STRING_CHECK(S)         \
assert( (S).addr_ );                    \
assert( (S).items>0 );                   \
assert( (S).bytes>=(S).items*sizeof(T) ); \
assert( (S).items-1==(S).maxi_ );          \
assert( (S).size_<=(S).maxi_ );             \
assert( 0 == (S).addr_[ (S).size_ ] )

    //! default fields initialisation
#define Y_CORE_STRING_CTOR0()      memory::rw_buffer(), dynamic(), addr_(0)

    //! string constructor to hold SIZE
#define Y_CORE_STRING_CTOR(SIZE)   Y_CORE_STRING_CTOR0(), size_(SIZE), maxi_(0), items(size_+1), bytes(0)

    //! string memory allocation and setup
#define Y_CORE_STRING_ALLOC()                                \
static memory::allocator &hmem = memory::pooled::instance(); \
addr_ = hmem.acquire_as<T>(items,bytes);                     \
maxi_ = items-1

    namespace core
    {
        //! string on a base class
        template <typename T>
        class string : public memory::rw_buffer, public dynamic
        {
        public:
            //! buffer interface
            inline virtual const void *ro()     const throw() { return addr_;  }

            //! buffer interface
            inline virtual size_t      length() const throw() { return size_*sizeof(T); }

            //! the size method...
            inline  virtual size_t size()     const throw() { return size_; }
            inline  virtual size_t capacity() const throw() { return maxi_; }
            
            //! destroy memory
            inline virtual ~string() throw()
            {
                Y_CORE_STRING_CHECK(*this);
                static memory::allocator &hmem = memory::pooled::location();
                hmem.release_as<T>(addr_,items,bytes);
            }

            //! set as empty
            inline void clear() throw() { addr_[(size_=0)]=0; }

            //! default constructor
            inline string() : Y_CORE_STRING_CTOR(0)
            {
                Y_CORE_STRING_ALLOC();
                Y_CORE_STRING_CHECK(*this);
            }

            //! copy constructor
            inline string(const string &other) :  Y_CORE_STRING_CTOR(other.size_)
            {
                Y_CORE_STRING_CHECK(other);
                Y_CORE_STRING_ALLOC();
                memcpy(addr_,other.addr_, size_*sizeof(T) );
                Y_CORE_STRING_CHECK(*this);
            }

            //! assignement of another string
            inline string & operator=(const string &other)
            {
                Y_CORE_STRING_CHECK(other);
                if(this!=&other)
                {
                    if(other.size_>maxi_)
                    {
                        string tmp(other);
                        swap_with(tmp);
                    }
                    else
                    {
                        memcpy(addr_,other.addr_,(size_=other.size_)*sizeof(T));
                        addr_[size_] = 0;
                        Y_CORE_STRING_CHECK(*this);
                    }
                }
                return *this;
            }

            //! assignement
            inline void assign(const char *s, const size_t n)
            {
                if(n<=maxi_)
                {
                    size_ = n;
                    for(size_t i=0;i<n;++i)
                    {
                        addr_[i] = s[i];
                    }
                    addr_[size_]=0;
                    Y_CORE_STRING_CHECK(*this);
                }
                else
                {
                    string tmp(s,n);
                    swap_with(tmp);
                }
            }

            //! assignement of a C-string
            inline string & operator=(const char *s)
            {
                assign(s,length_of(s));
                return *this;
            }
            

            //! no-throw swap
            inline void swap_with(string &other) throw()
            {
                cswap(addr_,other.addr_);
                cswap(size_,other.size_);
                cswap(maxi_,other.maxi_);
                cswap(items,other.items);
                cswap(bytes,other.bytes);
            }

            //! content operator
            inline T *       operator *() throw()       { return addr_; }

            //! content operator, CONST
            inline const T * operator *() const throw() { return addr_; }

            //! C-style constructor
            inline string( const T *s ) :  Y_CORE_STRING_CTOR(length_of(s))
            {
                Y_CORE_STRING_ALLOC();
                memcpy(addr_,s,size_*sizeof(T));
            }

            //! C-style with forced length buffer constructor
            inline string( const T *s, const size_t n ) : Y_CORE_STRING_CTOR(n)
            {
                Y_CORE_STRING_ALLOC();
                memcpy(addr_,s,size_*sizeof(T));
                Y_CORE_STRING_CHECK(*this);
            }

            //! memory reserve
            inline string( const size_t n, const as_capacity_t &) :
            Y_CORE_STRING_CTOR0(), size_(0), maxi_(0), items(n+1), bytes(0)
            {
                Y_CORE_STRING_ALLOC();
            }

            //! construct with a single char
            inline string( const T C ) : Y_CORE_STRING_CTOR(1)
            {
                Y_CORE_STRING_ALLOC();
                addr_[0] = C;
                Y_CORE_STRING_CHECK(*this);
            }

            //! standard display
            void std_display(std::ostream &) const;

            //! display operator
            inline friend std::ostream & operator<<( std::ostream &os, const string &s )
            {
                s.std_display(os);
                return os;
            }

            //! access operator
            inline T       & operator[](const size_t indx) throw()       { assert(indx<size_);  return addr_[indx]; }

            //! access operator, CONST, allow to read the final 0
            inline const T & operator[](const size_t indx) const throw() { assert(indx<=size_); return addr_[indx]; }

            //! in place addition
            inline string & operator+=( const string &rhs )
            {
                add(rhs.addr_,rhs.size_);
                return *this;
            }

            //! in place addition
            inline string & operator+=( const T *rhs )
            {
                add(rhs,length_of(rhs));
                return *this;
            }

            //! in place addition
            inline string & operator+=( const T C )
            {
                add(&C,1);
                return *this;
            }

            //! in place addition
            inline string & operator<<(const string &rhs)
            {
                add(rhs.addr_,rhs.size_);
                return *this;
            }

            //! in place addition
            inline string & operator<<( const T *rhs )
            {
                add(rhs,length_of(rhs));
                return *this;
            }

            //! in place addition
            inline string & operator<<( const T C )
            {
                add(&C,1);
                return *this;
            }


            //! addition
            inline friend string operator+( const string &lhs, const string &rhs )
            {
                return string(lhs.addr_,lhs.size_,rhs.addr_,rhs.size_);
            }

            //! addition
            inline friend string operator+(const string &lhs, const T *rhs )
            {
                return string(lhs.addr_,lhs.size_,rhs,length_of(rhs));
            }

            //! addition
            inline friend string operator+(const T *lhs, const string &rhs)
            {
                return string(lhs,length_of(lhs),rhs.addr_,rhs.size_);
            }

            //! addition
            inline friend string operator+( const string &lhs, const T C )
            {
                return string(lhs.addr_,lhs.size_,&C,1);
            }

            //! addition
            inline friend string operator+(const T C,const string &rhs)
            {
                return string(&C,1,rhs.addr_,rhs.size_);
            }

            //! lexicographic compare with size management
            static inline
            int compare_blocks( const T *sa, const size_t na, const T *sb, const size_t nb) throw()
            {
                return comparison::lexicographic<T>(sa,na,sb,nb);
            }

            //! lexicographic compare
            static inline
            int compare(const string &lhs, const string &rhs) throw()
            {
                return compare_blocks(lhs.addr_,lhs.size_,rhs.addr_,rhs.size_);
            }

            //! implement multiple semantic comparisons
#define Y_CORE_STRING_CMP(OP) \
inline friend bool operator OP ( const string &lhs, const string &rhs ) throw() { return string::compare_blocks(lhs.addr_,lhs.size_,rhs.addr_,rhs.size_) OP 0;}\
inline friend bool operator OP ( const string &lhs, const char *  rhs ) throw() { return string::compare_blocks(lhs.addr_,lhs.size_,rhs,length_of(rhs))  OP 0;}\
inline friend bool operator OP ( const char   *lhs, const string &rhs ) throw() { return string::compare_blocks(lhs,length_of(lhs),rhs.addr_,rhs.size_)  OP 0;}\
inline friend bool operator OP ( const string &lhs, const T       rhs ) throw() { return string::compare_blocks(lhs.addr_,lhs.size_,&rhs,1) OP 0;}             \
inline friend bool operator OP ( const T       lhs, const string &rhs ) throw() { return string::compare_blocks(&lhs,1,rhs.addr_,rhs.size_) OP 0;}

            Y_CORE_STRING_CMP(==)
            Y_CORE_STRING_CMP(!=)
            Y_CORE_STRING_CMP(<=)
            Y_CORE_STRING_CMP(<)
            Y_CORE_STRING_CMP(>=)
            Y_CORE_STRING_CMP(>)

            //! trim n last chars
            inline string & trim(const size_t n) throw()
            {
                addr_[ (size_ = (n>=size_) ? 0 : size_-n) ]=0;
                Y_CORE_STRING_CHECK(*this);
                return *this;
            }

        private:
            T     *addr_;
            size_t size_;
            size_t maxi_;
            size_t items;
            size_t bytes;

            //! build with two buffers
            inline string(const T *sa, const size_t na, const T *sb, const size_t nb) :
            addr_(0), size_( na+nb ), maxi_(0), items( size_+1 ), bytes(0)
            {
                Y_CORE_STRING_ALLOC();
                {
                    T *p = addr_;
                    for(size_t i=0;i<na;++i)
                    {
                        p[i] = sa[i];
                    }
                }

                {
                    T *p = &addr_[na];
                    for(size_t i=0;i<nb;++i)
                    {
                        p[i] = sb[i];
                    }
                }
                Y_CORE_STRING_CHECK(*this);
            }

            //! add a buffer
            inline void add(const T *s, const size_t n)
            {
                const size_t new_size = size_ + n;
                if(new_size<=maxi_)
                {
                    T       *p = &addr_[size_];
                    for(size_t i=0;i<n;++i)
                    {
                        p[i] = s[i];
                    }
                    addr_[ (size_ = new_size) ] = 0;
                }
                else
                {
                    string tmp(addr_,size_,s,n);
                    swap_with(tmp);
                }
                Y_CORE_STRING_CHECK(*this);
            }


        };
    }
}

#endif

