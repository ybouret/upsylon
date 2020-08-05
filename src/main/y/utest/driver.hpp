//! \file
#ifndef Y_UTEST_DRIVER_INCLUDED
#define Y_UTEST_DRIVER_INCLUDED 1

#include "y/exception.hpp"
#include "y/alea.hpp"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace upsylon
{
    //__________________________________________________________________________
    //
    //! class to handle unit tests
    //
    //__________________________________________________________________________
    struct utest
    {

        //______________________________________________________________________
        //
        //! external function to call
        typedef int (*func_type)( int argc, char **argv );
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! named function
        //______________________________________________________________________
        class proc_type
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! default constructor, set to empty
            inline  proc_type() throw() : func(0), name(0), size(0) {}
            
            //! default destructor
            inline ~proc_type() throw() { func=0; name=0; }
            
            //! direct constructor
            inline  proc_type(func_type f, const char *n) throw() : func(f), name(n), size(name?strlen(name):0) {}
            
            //! copy constructor
            inline  proc_type(const proc_type &other) throw() : func(other.func), name(other.name), size(other.size) {}
            
            //! assignment
            inline  proc_type & operator=(const proc_type &other) throw()
            {
                func          = other.func;
                name          = other.name;
                (size_t&)size = other.size;
                return *this;
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! libc style compare by name
            static int compare( const void *lhs, const void *rhs ) throw()
            {
                const proc_type *L = (const proc_type *)lhs;
                const proc_type *R = (const proc_type *)rhs;
                assert( NULL != L->name );
                assert( NULL != R->name );
                return strcmp( L->name, R->name );
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            func_type    func; //!< the function address
            const char  *name; //!< the function's name by user
            const size_t size; //!< the function's name length
        };

        //______________________________________________________________________
        //
        //! a suite of possible tests
        //______________________________________________________________________
        template <size_t N>
        class suite
        {
        public:
            inline  suite() throw() : reg_(), num_(0), max_size(0) {}
            inline ~suite() throw() {}

            //__________________________________________________________________
            //
            //! append a new test, then sort by name
            //__________________________________________________________________
            inline void operator()(func_type func, const char *name) throw()
            {
                assert( 0 != func );
                assert( 0 != name );
                if( num_ >= N )
                {
                    std::cerr << "Already got " << N << " tests!" << std::endl;
                    exit(-1);
                }

                if( search(name) )
                {
                    std::cerr << "Already got test '" << name << "'" << std::endl;
                    exit(-1);
                }

                const proc_type p(func,name);
                reg_[num_++] = p;
                qsort(reg_,num_,sizeof(proc_type),proc_type::compare);
                if(p.size>max_size) max_size=p.size;
            }

            //__________________________________________________________________
            //
            //! helper for centered name
            //__________________________________________________________________
            inline std::ostream & display(std::ostream &os, const proc_type &p) const
            {
                const size_t     len = p.size; assert(len<=max_size);
                const size_t     n   = max_size - len;
                const size_t     h   = n>>1 ;
                for(size_t j=0;j<h;++j) os << ' ';
                os << p.name;
                for(size_t j=h;j<n;++j) os << ' ';
                return os;
            }


            //__________________________________________________________________
            //
            //! called from main, execute a given test
            //__________________________________________________________________
            inline int operator()(int argc, char **argv)
            {
                if( argc <= 1)
                {
                    std::cerr << "List of #Tests=" << num_ << " in " << argv[0] << std::endl;
                    for( size_t i=0;i<num_;++i)
                    {
                        display( std::cout << '\t' << '[',reg_[i])  << ']' << std::endl;
                    }
                    std::cerr << "End of #Tests=" << num_ << " in " << argv[0] << std::endl;
                    return 1;
                }
                else
                {
                    const char      *name = argv[1];
                    const proc_type *proc = search( name );
                    if( !proc )
                    {
                        std::cerr << "-- I don't know how to '" << name << "'" << std::endl;
                        bool first = true;
                        for(size_t i=0;i<num_;++i)
                        {
                            const proc_type &p     = reg_[i];
                            const char      *guess = p.name;
                            const char      *match = strstr(guess,name);
                            if(match)
                            {
                                if(first)
                                {
                                    std::cerr << "-- But...I know how to :" << std::endl;
                                    first = false;
                                }
                                display(std::cerr << '\t' << '|',p) << '|' << std::endl;
                            }

                        }
                        return -1;
                    }
                    try
                    {
                        return proc->func(--argc,++argv);
                    }
                    catch( upsylon::exception &e )
                    {
                        std::cerr << e.when() << std::endl;
                        std::cerr << e.what() << std::endl;
                        return 1;
                    }
                    catch( std::exception &e )
                    {
                        std::cerr << e.what() << std::endl;
                        return 2;
                    }
                    catch(...)
                    {
                        std::cerr << "Unhandled exception in " << name << "!" << std::endl;
                        return 3;
                    }
                }
                return 0;
            }

        private:
            proc_type reg_[N];
            size_t    num_;
            size_t    max_size;
            Y_DISABLE_COPY_AND_ASSIGN(suite);
            const proc_type *search( const char *name ) const throw()
            {
                const proc_type lhs( NULL, name );
                return (proc_type *)bsearch( &lhs, reg_, num_, sizeof(proc_type), proc_type::compare);
            }

        };
    };
}

//! driver prolog
#define Y_UTEST_INIT(N)             \
/*    */    int main( int argc, char **argv )  \
/*    */    {    upsylon::utest::suite<N> tests;  srand( int(time(NULL)) ); upsylon::alea_init();

//! register a new test
#define Y_UTEST(NAME) do{                                \
/*    */        extern int upsylon_test_##NAME(int argc, char **argv); \
/*    */        tests( upsylon_test_##NAME, #NAME );                   \
/*    */    } while(0)

//! driver epilog
#define Y_UTEST_EXEC()     \
/*    */    return tests(argc,argv);   \
/*    */    }

#endif


