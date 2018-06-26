#ifndef Y_UTEST_DRIVER_INCLUDED
#define Y_UTEST_DRIVER_INCLUDED 1

#include "y/exception.hpp"
#include <cstring>
#include <cstdlib>
#include <iostream>

namespace upsylon
{
    struct utest
    {
        typedef int (*func_type)( int argc, char *argv[] );
        class proc_type
        {
        public:
            func_type   func;
            const char *name;
            inline  proc_type() : func(NULL), name(NULL) {}
            inline ~proc_type() throw() {}
            inline  proc_type( func_type f, const char *n) throw() : func(f), name(n) {}
            inline  proc_type( const proc_type &other ) throw() : func(other.func), name(other.name) {}
            inline  proc_type & operator=( const proc_type &other ) throw()
            {
                func = other.func;
                name = other.name;
                return *this;
            }

            static int compare( const void *lhs, const void *rhs ) throw()
            {
                const proc_type *L = (const proc_type *)lhs;
                const proc_type *R = (const proc_type *)rhs;
                assert( NULL != L->name );
                assert( NULL != R->name );
                return strcmp( L->name, R->name );
            }
        };

        template <size_t N>
        class suite
        {
        public:
            inline  suite() throw() : reg_(), num_(0) {}
            inline ~suite() throw() {}

            inline void operator()( func_type func, const char *name ) throw()
            {
                assert( NULL != func );
                assert( NULL != name );
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

            inline int operator()(int argc, char *argv[])
            {
                if( argc <= 1)
                {
                    std::cerr << "List of Tests in " << argv[0] << std::endl;
                    for( size_t i=0; i < num_; ++i )
                    {
                        std::cout << "--  " << reg_[i].name << std::endl;
                    }
                    return 1;
                }
                else
                {
                    const char      *name = argv[1];
                    const proc_type *proc = search( name );
                    if( !proc )
                    {
                        std::cerr << "-- unknown test '" << name << "'" << std::endl;
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

#define Y_UTEST_INIT(N)             \
/*    */    int main( int argc, char *argv[] )  \
/*    */    {    upsylon::utest::suite<N> tests;   

#define Y_UTEST(NAME) do{                                \
/*    */        extern int upsylon_test_##NAME(int argc, char **argv); \
/*    */        tests( upsylon_test_##NAME, #NAME );                   \
/*    */    } while(0)

#define Y_UTEST_EXEC()     \
/*    */    return tests(argc,argv);   \
/*    */    }

#endif


