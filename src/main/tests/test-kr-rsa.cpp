#include "y/yap/integer.hpp"
#include "y/utest/run.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/string.hpp"
#include "y/string/tokenizer.hpp"

using namespace upsylon;

namespace
{

    static inline bool is_blank(char C) throw()
    {
        return C == ' ' || C == '\t';
    }

    class KeyFile
    {
    public:
        explicit KeyFile(ios::istream &fp)
        {
            vector<string,memory::pooled> fields;
            {
                string line;

                while(fp.gets(line))
                {
                    if(line.size()<=0) continue;
                    if(is_blank(line[0]))
                    {
                        if(fields.size()<=0) throw exception("no field!");

                        line.clean(is_blank);
                        fields.back() += line;
                    }
                    else
                    {
                        fields.push_back(line);
                    }
                }

            }

            if(fields.size()<=3) throw exception("not enough fields");
            fields.pop_front();

            {
                vector<string,memory::pooled> words(1024,as_capacity);
                for(size_t i=1;i<=fields.size();++i)
                {
                    tokenizer<char>::split_with(words,fields[i],':');
                    std::cerr << "[" << i << "] " << words << std::endl;
                    if(words.size()<=1) throw exception("bla bla");


                }
            }
        }

        virtual ~KeyFile() throw()
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(KeyFile);

    };
}

Y_UTEST(kr_rsa)
{

    if(argc>1)
    {
        const string  filename = argv[1];
        ios::icstream fp(filename);

        KeyFile kf(fp);



    }
}
Y_UTEST_DONE()

