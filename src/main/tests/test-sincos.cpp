#include "y/utest/run.hpp"

namespace
{
    template <typename T> inline
    void callsincos(T &s, T &c, const T alpha) throw()
    {
#undef Y_FSINCOS_ASM

#if defined(__GNUC__)
#    if defined(__x86_64__) || defined(_X86_)
#        define Y_FSINCOS_ASM 1
        asm ( "fsincos" : "=t" (c), "=u" (s) : "0" (alpha) );
#    endif
#endif

#if  defined(_MSC_VER)
#if 0
#    define Y_FSINCOS_ASM 1
		_asm {
			finit
			fld DWORD PTR alpha
			fsincos
			fstp DWORD PTR c
			fstp DWORD PTR s
			fwait
	}
#endif
#endif

#if !defined(Y_FSINCOS_ASM)
        s=sin(alpha);
        c=cos(alpha);
#endif
    }

    template <typename T>
    void test_fsincos()
    {
        for(T alpha=T(-6.3);alpha<=T(6.3);alpha+=T(0.1))
        {
            const T c = cos(alpha);
            const T s = sin(alpha);
            T cc=0,ss=0;
            callsincos(ss,cc,alpha);
            std::cerr << alpha << " => cos: " << cc << "/" << c << " sin : " << ss << "/" << s << std::endl;
        }
    }
}




Y_UTEST(sincos)
{
    test_fsincos<float>();
   // test_fsincos<double>();

#if 0
	double x = 1.0;
	double sinx=0, cosx=0;

	/* 
	 * Inline the "fsincos" x86 intruction: fsincos pops the value
	 * at the top of the FP stack, and pushes on the same stack
	 * the computed values of its sine and cosine.
	 *
	 *  "=t" is an output (sin x) operand, at the top of the FP stack.
	 *
	 *  "=u" is an output (cos x) operand, next-to-top on the FP stack.
	 *
	 *  "0"  is an input (x) operand, identified with the first output
	 *       operand ("=t"), located at the top of the FP stack.
	 */

	/* display results */
	printf("x=%e : sin(x)=%e, cos(x)=%e\n", x, sinx, cosx);
#endif


}
Y_UTEST_DONE()
