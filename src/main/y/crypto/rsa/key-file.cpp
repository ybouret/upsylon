
#include "y/crypto/rsa/key-file.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/exception.hpp"
#include "y/ios/align.hpp"

namespace upsylon
{
    namespace crytpo
    {

        key_file:: ~key_file() throw()
        {
        }

        static inline bool is_blank(const int C) throw()
        {
            return C == ' ' || C == '\t';
        }


        key_file:: key_file(ios::istream &fp) : named_keys(8,as_capacity)
        {
            typedef vector<string,memory::pooled> strings;
            static const char fn[] = "crypto::key_file: ";

            strings fields;
            {
                //--------------------------------------------------------------
                //
                // read fields
                //
                //--------------------------------------------------------------
                string line;
                while(fp.gets(line))
                {
                    if(line.size()<=0) continue;

                    if(is_blank(line[0]))
                    {
                        //------------------------------------------------------
                        // start with a blank => add content to current field
                        //------------------------------------------------------
                        if(fields.size()<=0) throw exception("%smissing first field",fn);

                        line.clean(is_blank);
                        fields.back() += line;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // start a new field
                        //------------------------------------------------------
                        fields.push_back(line);
                    }
                }

                //--------------------------------------------------------------
                //
                // first field is info, need at least modulus and exponent
                //
                //--------------------------------------------------------------
                if(fields.size()<=3) throw exception("%snot enough fields",fn);
                fields.pop_front();


                //--------------------------------------------------------------
                //
                // parse fields
                //
                //--------------------------------------------------------------
                {
                    strings words(1024,as_capacity);
                    for(size_t i=1;i<=fields.size();++i)
                    {
                        if( tokenizer<char>::split_with(words,fields[i],':') <= 1) throw exception("%sempty field!!",fn);

                        const string name = words[1];
                        std::cerr << ios::align(name,ios::align::left,16) << " => ";
                        apn          data;
                        if(words.size()==2)
                        {
                            // decimal info
                            const string temp = words[2];
                            if( tokenizer<char>::split_with(words,temp,' ') != 2) throw exception("%sinvalid decimal+hex format",fn);
                            data = apn::dec(words[1]);
                        }
                        else
                        {
                            // hexadecimal
                            for(size_t j=2;j<=words.size();++j)
                            {
                                const apn b = apn::hex(words[j]);
                                if(b>255) throw exception("%sinvalid byte!!",fn);
                                data <<= 8;
                                data +=  b;
                            }
                        }
                        std::cerr << std::hex;
                        std::cerr << data;
                        std::cerr << std::dec;

                        if(!insert(name,data)) throw exception("%smultiple field '%s'",fn,*name);
                        std::cerr << std::endl;
                    }
                }

            }

        }
        

    }
}
