
#include "y/concurrent/task/worker.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;

namespace
{

    template <typename T>
    class ref_to
    {
    public:
        inline ref_to(T &host) : ref(host)
        {
        }

        inline virtual ~ref_to() throw()
        {
        }

        T * operator->() throw() { return &ref; }
        T & operator*() throw()  { return ref; }
    private:
        T &ref;
    };

    class Dummy : public ref_to<string>
    {
    public:
        Dummy(string &s) : ref_to<string>(s)
        {
        }

        virtual ~Dummy() throw()
        {
        }

    private:

    };


}

Y_UTEST(thr_team)
{
    Y_UTEST_SIZEOF(concurrent::worker);
    Y_UTEST_SIZEOF(concurrent::condition);

    //concurrent::team Q;

    string s = "hello";
    {
        Dummy dummy(s);
        std::cerr << "size=" << dummy->size() << " : " << *dummy << std::endl;

    }
}
Y_UTEST_DONE()

