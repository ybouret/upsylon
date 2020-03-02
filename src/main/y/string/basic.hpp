//! \file
#ifndef Y_STRING_BASIC_INCLUDED
#define Y_STRING_BASIC_INCLUDED 1

#include "y/memory/buffer.hpp"
#include "y/type/cswap.hpp"
#include "y/dynamic.hpp"
#include "y/comparison.hpp"
#include "y/ptr/counted.hpp"
#include "y/ios/serializable.hpp"
#include "y/core/chainable.hpp"
#include "y/os/oor.hpp"

#include <cstring>
#include <iosfwd>

namespace upsylon {

    namespace ios {
        class istream; //!< forward declaration
    }

    ////////////////////////////////////////////////////////////////////////////
    //
    // utility functions
    //
    ////////////////////////////////////////////////////////////////////////////

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

    ////////////////////////////////////////////////////////////////////////////
    //
    // core string macros
    //
    ////////////////////////////////////////////////////////////////////////////

#if defined(NDEBUG)
#define Y_CORE_STRING_CHECK(S)
#else
    //! sanity check of a core string
#define Y_CORE_STRING_CHECK(S)                     \
assert( (S).addr_ );                                \
assert( (S).items>0 );                               \
assert( (S).bytes>=(S).items*sizeof(T) );             \
assert( (S).items-1==(S).maxi_ );                      \
assert( (S).size_<=(S).maxi_ );                         \
assert( 0 == (S).addr_[ (S).size_ ] );                   \
do { for(size_t izero=(S).size_;izero<=(S).maxi_;++izero) \
{ assert(0==(S).addr_[izero]); } } while(false)
#endif

    //! default fields initialisation
#define Y_CORE_STRING_CTOR0()      counted_object(), collection(), memory::rw_buffer(), dynamic(), ios::serializable(), upsylon::chainable<T>(), addr_(0)

    //! string constructor to hold SIZE
#define Y_CORE_STRING_CTOR(SIZE)   Y_CORE_STRING_CTOR0(), size_(SIZE), maxi_(0), items(size_+1), bytes(0)

    //! string memory allocation and setup
#define Y_CORE_STRING_ALLOC()                                 \
static memory::allocator &hmem = string_allocator_instance(); \
addr_ = hmem.acquire_as<T>(items,bytes);                      \
maxi_ = items-1

    //! string padding with zero after a random operation
#define Y_CORE_STRING_ZPAD() do { assert(bytes>=sizeof(T)*size_); memset(addr_+size_,0,bytes-sizeof(T)*size_); } while(false)


    ////////////////////////////////////////////////////////////////////////////
    //
    // core string implementation
    //
    ////////////////////////////////////////////////////////////////////////////

    namespace core {
        memory::allocator & string_allocator_instance();          //!< call to internal pooled memory
        memory::allocator & string_allocator_location() throw();  //!< call to internal pooled memory

        //! string on a base class
        /**
         assuming that all chars after size() are '0'
         */
        template <typename T>
        class string :
        public virtual counted_object,
        public         memory::rw_buffer,
        public dynamic,
        public ios::serializable,
        public upsylon::chainable<T>
        {
        public:
            static const char CLASS_NAME[]; //!< for serializable

            //! buffer interface
            inline virtual const void *ro()     const throw() { return addr_;  }

            //! buffer interface
            inline virtual size_t      length() const throw() { return size_*sizeof(T); }

            //! dynamic interface: the size method...
            inline  virtual size_t size()     const throw() { return size_; }

            //! dynamic interface: the capacity method
            inline  virtual size_t capacity() const throw() { return maxi_; }
            
            //! destroy memory
            inline virtual ~string() throw()
            {
                Y_CORE_STRING_CHECK(*this);
                static memory::allocator &hmem = string_allocator_location();
                hmem.release_as<T>(addr_,items,bytes);
            }

            //! set as empty
            inline void clear() throw() { force(0); }

            //! force size
            inline void force( const size_t n ) throw()
            {
                Y_CORE_STRING_CHECK(*this);
                while(size_>n)
                {
                    addr_[--size_]=0;
                }
                Y_CORE_STRING_CHECK(*this);
            }

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
                        Y_CORE_STRING_ZPAD();
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
                    Y_CORE_STRING_ZPAD();
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
                Y_CORE_STRING_CHECK(*this);
                Y_CORE_STRING_CHECK(other);
                cswap(addr_,other.addr_);
                cswap(size_,other.size_);
                cswap(maxi_,other.maxi_);
                cswap(items,other.items);
                cswap(bytes,other.bytes);
                Y_CORE_STRING_CHECK(*this);
                Y_CORE_STRING_CHECK(other);
            }

            //! content operator
            inline T *       operator *() throw()       { return addr_; }

            //! content operator, CONST
            inline const T * operator *() const throw() { return addr_; }

            //! C-style constructor
            inline string( const T *s ) :  Y_CORE_STRING_CTOR(length_of(s))
            {
                Y_CORE_STRING_ALLOC();
                out_of_reach::copy(addr_,s,size_ * sizeof(T) );
                Y_CORE_STRING_CHECK(*this);
            }

            //! C-style with forced length buffer constructor
            inline string( const T *s, const size_t n ) : Y_CORE_STRING_CTOR(n)
            {
                Y_CORE_STRING_ALLOC();
                out_of_reach::copy(addr_,s,size_ * sizeof(T) );
                Y_CORE_STRING_CHECK(*this);
            }

            //! memory reserve
            inline string( const size_t n, const as_capacity_t &, bool filled) :
            Y_CORE_STRING_CTOR0(), size_(0), maxi_(0), items(n+1), bytes(0)
            {
                Y_CORE_STRING_ALLOC();
                Y_CORE_STRING_CHECK(*this);
                if(filled) size_ = n;
                Y_CORE_STRING_CHECK(*this);
            }

            //! construct with a single char
            inline string( const T C ) : Y_CORE_STRING_CTOR(1)
            {
                Y_CORE_STRING_ALLOC();
                addr_[0] = C;
                Y_CORE_STRING_CHECK(*this);
            }

            //! standard display
            std::ostream & std_display(std::ostream &) const;

            //! display operator
            inline friend std::ostream & operator<<( std::ostream &os, const string &s )
            {
                return s.std_display(os);
            }
            
            //! visible chars
            std::ostream & display_visible(std::ostream &) const;

            //! printable chars
            std::ostream & display_printable(std::ostream &s) const;


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

            //! macro implementing all the comparisons
#define Y_CORE_STRING_CMP_IMPL()  \
Y_CORE_STRING_CMP(==) \
Y_CORE_STRING_CMP(!=) \
Y_CORE_STRING_CMP(<=) \
Y_CORE_STRING_CMP(<)  \
Y_CORE_STRING_CMP(>=) \
Y_CORE_STRING_CMP(>)

            Y_CORE_STRING_CMP_IMPL()

            //! check address
            inline bool owns( const T *p ) const throw()
            {
                return (p>=addr_) && (p<addr_+items);
            }

            //! trim n last chars
            inline string & trim(size_t n) throw()
            {
                Y_CORE_STRING_CHECK(*this);
                while(n-->0 && size_>0)
                {
                    addr_[--size_] = 0;
                }
                Y_CORE_STRING_CHECK(*this);
                return *this;
            }

            //! trim all bad chars
            template <typename FUNC> //bool (*is_bad)(const char C)
            inline string &trim( FUNC &is_bad ) throw()
            {
                Y_CORE_STRING_CHECK(*this);
                while(size_>0)
                {
                    const size_t i = size_-1;
                    if(!is_bad(addr_[i]))
                    {
                        break;
                    }
                    else
                    {
                        addr_[ (size_=i) ]=0;
                    }
                }
                Y_CORE_STRING_CHECK(*this);
                return *this;
            }

            //! trim with some chars
            inline string & trim_with( const T *buf, const size_t len ) throw()
            {
                assert(!owns(buf));
                const is_bad_function is_bad = { buf, len };
                return trim(is_bad);
            }

            //! trim with some chars
            inline string & trim_with( const T *buf ) throw()
            {
                return trim(buf, length_of(buf));
            }

            //! trim with one bad char
            inline string & trim_with( const T c ) throw()
            {
                return trim_with(&c,1);
            }


            //! skip n first chars
            inline string & skip(const size_t n) throw()
            {
                Y_CORE_STRING_CHECK(*this);
                if(n>=size_)
                {
                    clear();
                }
                else
                {
                    assert(n<size_);
                    const size_t old_size=size_;
                    size_ -= n;
                    for(size_t i=0,j=n;i<size_;++i,++j)
                    {
                        addr_[i] = addr_[j];
                    }
                    for(size_t i=size_;i<=old_size;++i)
                    {
                        assert(i<items);
                        addr_[i] = 0;
                    }
                    Y_CORE_STRING_CHECK(*this);
                }
                return *this;
            }

            //! skip first 'is_bad' chars
            template <typename FUNC>
            inline string & skip( FUNC &is_bad ) throw()
            {
                size_t n = 0;
                for(size_t i=0;i<size_;++i)
                {
                    if(is_bad(addr_[i]))
                    {
                        ++n;
                    }
                    else
                    {
                        break;
                    }
                }
                return skip(n);
            }

            //! skip with buffer
            inline string & skip_with( const T *buf, const size_t len ) throw()
            {
                assert( !owns(buf) );
                const is_bad_function is_bad = { buf, len };
                return skip(is_bad);
            }

            //! skip with text
            inline string & skip_with( const T *buf  ) throw()
            {
                return skip_with(buf,length_of(buf));
            }

            //! skip with single char
            inline string & skip_with( const T c ) throw()
            {
                return skip_with(&c,1);
            }


            //! remove first and last bad chars
            template <typename FUNC>
            inline string & clean( FUNC &is_bad ) throw()
            {
                return trim(is_bad).skip(is_bad);
            }

            //! clean with buffer
            inline string & clean_with( const T *buf, const size_t len ) throw()
            {
                assert( !owns(buf) );
                const is_bad_function is_bad = { buf, len };
                return clean(is_bad);
            }

            //! clean with text
            inline string & clean_with( const T *buf ) throw()
            {
                return clean_with( buf, length_of(buf) );
            }

            //! clean with single char
            inline string & clean_with( const T c ) throw()
            {
                return clean_with( &c, 1);
            }



            //! back operator
            inline T &       back() throw()        { assert(size_>0); return addr_[size_-1]; }

            //! back, csont
            inline const T & back() const throw()  { assert(size_>0); return addr_[size_-1]; }

            //! front
            inline T &       front() throw()       { assert(size_>0); return addr_[0]; }

            //! front, const
            inline const T & front() const throw() { assert(size_>0); return addr_[0]; }

            //! serializable className
            virtual const char * className() const throw();

            //! write
            virtual size_t serialize( ios::ostream &fp ) const;

            //! reload
            static string      read(ios::istream &fp, size_t *shift);

            //! compact in memory
            bool compact() throw();

            //! self key
            inline const string & key() const throw() { return *this; }

            //! use the add function 
            inline virtual size_t put( const T *p, const size_t n )
            {
                add(p,n);
                return n;
            }

        private:
            T     *addr_;
            size_t size_;
            size_t maxi_;
            size_t items;
            size_t bytes;

            struct is_bad_function
            {
                const char  *buffer;
                size_t       buflen;
                inline bool operator()(const T c) const throw()
                {
                    assert(!(0==buffer&&buflen>0));
                    for(size_t i=buflen;i>0;--i)
                    {
                        if(c==buffer[i]) return true;
                    }
                    return false;
                }
            };

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
                Y_CORE_STRING_CHECK(*this);
                const size_t new_size = size_ + n;
                if(new_size<=maxi_)
                {
                    T       *p = &addr_[size_];
                    for(size_t i=0;i<n;++i)
                    {
                        p[i] = s[i];
                    }
                    addr_[ (size_ = new_size) ] = 0;
                    Y_CORE_STRING_CHECK(*this);
                }
                else
                {
                    string tmp(addr_,size_,s,n);
                    swap_with(tmp);
                }
            }


        };
    }
}

#endif

