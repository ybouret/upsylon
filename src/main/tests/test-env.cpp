#include "y/string/env.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;

namespace
{
    class envPair
    {
    public:
        const string name;
        string       value;
        inline envPair(const string &n, const string &v) :
        name(n), value(v)
        {
        }

        inline envPair(const envPair &other) :
        name(other.name), value(other.value)
        {

        }

        inline ~envPair() throw()
        {

        }

    private:
        Y_DISABLE_ASSIGN(envPair);
    };

    static inline void callBack(const string &name, const string &value, void *args)
    {
        list<envPair> &L = *static_cast< list<envPair> * >(args);
        const envPair  p(name,value);
        L.push_back(p);
        std::cerr << name << "=" << value << std::endl;
    }

}

Y_UTEST(env)
{
	
    list<envPair> envList;
	{
		string temp = "Library";
		environment::set("UPSYLON", *temp);
		temp.clear();
	}

	{
		string value;
		Y_CHECK(environment::get(value, "UPSYLON"));
		Y_CHECK("Library" == value);
	}
	
    environment::get(callBack,&envList);
    for( list<envPair>::iterator i=envList.begin();i!=envList.end();++i)
    {
        const envPair &p = *i;
        string         value;
        std::cerr << p.name << ": '" << p.value << "'" << std::endl;
        Y_CHECK(environment::get(value,p.name) );
        Y_CHECK(value==p.value);
    }

}
Y_UTEST_DONE()

