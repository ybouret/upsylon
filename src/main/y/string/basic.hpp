#ifndef Y_STRING_BASIC_INCLUDED
#define Y_STRING_BASIC_INCLUDED 1

#include "y/memory/pooled.hpp"
#include "y/memory/buffer.hpp"
#include "y/type/cswap.hpp"
#include <cstring>
#include <iosfwd>

namespace upsylon
{
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

#define Y_CORE_STRING_CHECK(S)         \
assert( (S).addr_ );                    \
assert( (S).items>0 );                   \
assert( (S).bytes>=(S).items*sizeof(T) ); \
assert( (S).items-1==(S).maxi_ );          \
assert( (S).size_<=(S).maxi_ )

#define Y_CORE_STRING_ALLOC()                                \
static memory::allocator &hmem = memory::pooled::instance(); \
addr_ = hmem.acquire_as<T>(items,bytes);                     \
maxi_ = items-1

    namespace core
    {
        template <typename T>
        class string : public memory::rw_buffer
        {
        public:
            //! buffer interface
            inline virtual const void *ro()     const throw() { return addr_;  }

            //! buffer interface
            inline virtual size_t      length() const throw() { return size_*sizeof(T); }

            //! the size method...
            inline  size_t size()     const throw() { return size_; }
            inline  size_t capacity() const throw() { return maxi_; }
            
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
            inline string() : addr_(0), size_(0), maxi_(0), items(1), bytes(0)
            {
                Y_CORE_STRING_ALLOC();
            }

            //! copy constructor
            inline string(const string &other) : addr_(0), size_(other.size_), maxi_(0), items(size_+1), bytes(0)
            {
                Y_CORE_STRING_ALLOC();
                memcpy(addr_,other.addr_, size_*sizeof(T) );
            }

            //! assignement of another string
            inline string & operator=(const string &other)
            {
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
                    }
                }
                return *this;
            }

            //! assignement of a C-string
            inline string & operator=(const char *s)
            {
                const size_t n = length_of(s);
                if(n<=maxi_)
                {
                    size_ = n;
                    for(size_t i=0;i<n;++i)
                    {
                        addr_[i] = s[i];
                    }
                    addr_[size_]=0;
                }
                else
                {
                    string tmp(s,n);
                    swap_with(tmp);
                }
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
            inline string( const T *s ) : addr_(0), size_( length_of(s) ), maxi_(0), items( size_+1 ), bytes(0)
            {
                Y_CORE_STRING_ALLOC();
                memcpy(addr_,s,size_*sizeof(T));
            }

            //! C-style with forced length buffer constructor
            inline string( const T *s, const size_t n ) : addr_(0), size_( n ), maxi_(0), items( size_+1 ), bytes(0)
            {
                Y_CORE_STRING_ALLOC();
                memcpy(addr_,s,size_*sizeof(T));
            }

            //! memory reserve
            inline string( const size_t n, const as_capacity_t &) :
            addr_(0), size_(0), maxi_(0), items(n+1), bytes(0)
            {
                Y_CORE_STRING_ALLOC();
            }

            inline string( char C ) : addr_(0), size_( 1 ), maxi_(0), items( 2 ), bytes(0)
            {
                Y_CORE_STRING_ALLOC();
                addr_[0] = C;
            }

            void std_display(std::ostream &) const;

            inline friend std::ostream & operator<<( std::ostream &os, const string &s )
            {
                s.std_display(os);
                return os;
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
            }
        };
    }
}

#endif

