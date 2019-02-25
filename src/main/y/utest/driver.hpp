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
    //! class to handle unit tests
    struct utest
    {
        //! external function to call
        typedef int (*func_type)( int argc, char **argv );
        
        //! named function
        class proc_type
        {
        public:
            func_type   func; //!< the function address
            const char *name; //!< the function's name by user
            
            //! default constructor, set to empty
            inline  proc_type() : func(0), name(0) {}
            
            //! default destructor
            inline ~proc_type() throw() { func=0; name=0; }
            
            //! direct constructor
            inline  proc_type( func_type f, const char *n) throw() : func(f), name(n) {}
            
            //! copy constructor
            inline  proc_type( const proc_type &other ) throw() : func(other.func), name(other.name) {}
            
            //! assignment
            inline  proc_type & operator=( const proc_type &other ) throw()
            {
                func = other.func;
                name = other.name;
                return *this;
            }

            //! libc style compare by name
            static int compare( const void *lhs, const void *rhs ) throw()
            {
                const proc_type *L = (const proc_type *)lhs;
                const proc_type *R = (const proc_type *)rhs;
                assert( NULL != L->name );
                assert( NULL != R->name );
                return strcmp( L->name, R->name );
            }
        };

        //! a suite of possible tests
        template <size_t N>
        class suite
        {
        public:
            inline  suite() throw() : reg_(), num_(0) {}
            inline ~suite() throw() {}

            //! append a new test, then sort by name
            inline void operator()( func_type func, const char *name ) throw()
            {
                assert( 0 != func );
                assert( 0 != name );
                if( num_ >= N )
                {
                    std::cerr << "Already got " << N << " tests!" << std::endl;
                    exit(-1);
                }

                if( search( name ) )
                {
                    std::cerr << "Already got test '" << name << "'" << std::endl;
                    exit(-1);
                }

                const proc_type p(func,name);
                reg_[num_++] = p;
                qsort(reg_,num_,sizeof(proc_type),proc_type::compare);
            }

            //! called from main, execute a given test
            inline int operator()(int argc, char **argv)
            {
                if( argc <= 1)
                {
                    std::cerr << "List of #Tests=" << num_ << " in " << argv[0] << std::endl;
                    for( size_t i=0; i < num_; ++i )
                    {
                        std::cout << "--  " << reg_[i].name << std::endl;
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


