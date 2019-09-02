
#include "y/utest/run.hpp"
#include "y/code/arith.hpp"
#include "y/memory/global.hpp"

using namespace upsylon;

namespace
{
    class Comb
    {
    public:
        
        static inline size_t Count( const size_t nn, const size_t kk)
        {
            size_t num = 1;
            size_t den = 1;
            const size_t dlo = 1;
            const size_t dhi = kk;
            const size_t nlo = nn+1-kk;
            const size_t nhi = nn;
            
            size_t i=nlo;
            size_t j=dlo;
            while(i<=nhi&&j<=dhi)
            {
                num *= i++;
                den *= j++;
                arithmetic::simplify(num,den);
            }
            
            while(j<=dhi)
            {
                den *= j++;
                arithmetic::simplify(num,den);
            }
            
            while(i<=nhi)
            {
                num *= i++;
                arithmetic::simplify(num,den);
            }
            
                
            assert(den==1);
            return num;
        }
        
        const size_t count;
        const size_t index;
        const size_t n;
        const size_t k;
        
        explicit Comb(const size_t nn, const size_t kk) :
        count( Count(nn,kk) ),
        index( 0 ),
        n(nn),
        k(kk),
        nb(0),
        nc(k),
        comb( memory::global::instance().acquire_as<size_t>(nc,nb) )
        {
        }
        
        ~Comb() throw()
        {
            memory::global::location().release_as<size_t>(comb,nc,nb);
        }
        
        void start() throw()
        {
            for(size_t i=0;i<k;++i)
            {
                comb[i] = i;
            }
            (size_t&)index = 1;
        }
        
        bool active() const throw()
        {
            assert(index>0);
            return (index<count);
        }
        
        bool next() throw()
        {
            assert(index<count);
            size_t i = k - 1;
            ++comb[i];
            while ((i >= 0) && (comb[i] >= n - k + 1 + i)) {
                --i;
                ++comb[i];
            }
            
            if (comb[0] > n - k) /* Combination (n-k, n-k+1, ..., n) reached */
                return false; /* No more combinations can be generated */
            
            /* comb now looks like (..., x, n, n, n, ..., n).
             Turn it into (..., x, x + 1, x + 2, ...) */
            for (i = i + 1; i < k; ++i)
            {
                comb[i] = comb[i - 1] + 1;
            }
            
            ++(size_t&)index;
            return true;
        }
        
        void printc() const
        {
            printf("{");
            size_t i;
            for (i = 0; i < k; ++i)
                printf("%u, ", unsigned(comb[i] + 1));
            printf("\b\b}\n");
        }
        
    private:
        size_t  nb;
        size_t  nc;
        size_t *comb;
        
        Y_DISABLE_ASSIGN(Comb);
    };
    
#if 0
    /* Prints out a combination like {1, 2} */
    static void printc(int comb[], int k) {
        printf("{");
        int i;
        for (i = 0; i < k; ++i)
            printf("%d, ", comb[i] + 1);
        printf("\b\b}\n");
    }
    
    /*
     next_comb(int comb[], int k, int n)
     Generates the next combination of n elements as k after comb
     
     comb => the previous combination ( use (1, 2, ..., k) for first)
     k => the size of the subsets to generate
     n => the size of the original set
     
     Returns: 1 if a valid combination was found
     0, otherwise
     */
    static int next_comb(int comb[], int k, int n) {
        int i = k - 1;
        ++comb[i];
        while ((i >= 0) && (comb[i] >= n - k + 1 + i)) {
            --i;
            ++comb[i];
        }
        
        if (comb[0] > n - k) /* Combination (n-k, n-k+1, ..., n) reached */
            return 0; /* No more combinations can be generated */
        
        /* comb now looks like (..., x, n, n, n, ..., n).
         Turn it into (..., x, x + 1, x + 2, ...) */
        for (i = i + 1; i < k; ++i)
            comb[i] = comb[i - 1] + 1;
        
        return 1;
    }
#endif
}

#include "y/string/convert.hpp"

Y_UTEST(comb)
{
    
    int n = 5; /* The size of the set; for {1, 2, 3, 4} it's 4 */
    int k = 3; /* The size of the subsets; for {1, 2}, {1, 3}, ... it's 2 */
   
    
    if(argc>1) n = string_convert::to<unit_t>(argv[1]); Y_ASSERT(n>0);
    if(argc>2) k = string_convert::to<unit_t>(argv[2]); Y_ASSERT(k>0); Y_ASSERT(k<=n);
    
#if 0
    int comb[16]; /* comb[i] is the index of the i-th element in the
                   combination */
    /* Setup comb for the initial combination */
    int i;
    for (i = 0; i < k; ++i)
        comb[i] = i;
    
    /* Print the first combination */
    printc(comb, k);
    
    /* Generate and print all the other combinations */
    while (next_comb(comb, k, n))
        printc(comb, k);
#endif
    
    std::cerr << "#=" << Comb::Count(n,k) << std::endl;
    
    Comb loop(n,k);
    for( loop.start(); loop.active(); loop.next() )
    {
        std::cerr << "#" << loop.index << ": ";
        loop.printc();
    }
    std::cerr << "#=" << Comb::Count(n,k) << std::endl;

}
Y_UTEST_DONE()


