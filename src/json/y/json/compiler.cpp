#include "y/json/compiler.hpp"
#include "y/lang/dynamox.hpp"
#include "y/string/convert.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace
    {
#include "json.inc"
    }

    namespace JSON
    {
#include "json.def"
        
        class Compiler : public Lang::DynamoX, public singleton<Compiler>
        {
        public:
            Array        vstack;
            vector<Pair> pstack;
            
            void  createFrom( Lang::Module *module )
            {
                // initialize
                pstack.free();
                vstack.free();
                
                compile(module);
                std::cerr << "vstack=" << vstack << std::endl;
            }

            virtual void onTerminal( const Lang::Lexeme &data )
            {
                Analyzer::onTerminal(data);
                const int code = terminalHash(*data.label);
                //space(std::cerr) << "code = 0x" << std::hex << code << std::dec << std::endl;
                switch(code)
                {
                    case JSON_null:   vstack.push(NullType); break;
                    case JSON_true:   vstack.push(TrueType); break;
                    case JSON_false:  vstack.push(TrueType); break;
                    case JSON_string: {
                        const string s = data.to_string(1,1);
                        vstack.push(s);
                    } break;
                    case JSON_number:
                    {
                        const string s = data.to_string();
                        const Number x = string_convert::to<double>(s,"JSON::Number");
                        vstack.push(x);
                    } break;
                    default:
                        break;
                }
            }
            
            virtual void onInternal(const string &name, const size_t size)
            {
                Analyzer::onInternal(name,size);
                const int code = internalHash(name);
                space(std::cerr) << "code = 0x" << std::hex << code << std::dec << std::endl;
                switch(code)
                {
                    case JSON_pair: {
                        if(size!=2)         throw exception("{%s}.%s invalid size!",**(parser->name),*name);
                        if(vstack.size()<2) throw exception("{%s}.%s missing stack value",**(parser->name),*name);
                       
                        Value v; v.swap_with(vstack.back()); vstack.pop();
                        Value l; l.swap_with(vstack.back()); vstack.pop();
                        if(l.type!=IsString) throw exception("{%s}.%s invalid label type",**(parser->name),*name);
                        const string s = l.as<String>();
                        Pair p = new _Pair(s,v);
                        p->value.swap_with(v);
                        pstack.push_back(p);
                        std::cerr << "(" << p->label << "," << p->value << ")" << std::endl;
                    } break;
                        
                    case JSON_heavy_object:
                    {
                        if(size<=0)            throw exception("{%s}.%s invalid size!",**(parser->name),*name);
                        if(pstack.size()<size) throw exception("{%s}.%s missing pairs",**(parser->name),*name);
                        JSON::Object &obj = vstack.push(ObjectType);
                        for(size_t i=0;i<size;++i)
                        {
                            const Pair p = pstack.back();
                            if(!obj.insert(p))
                            {
                                throw exception("{%s}.%s mutliple label '%s",**(parser->name),*name,*(p->label));
                            }
                        }
                    } break;
                        
                    case JSON_empty_object:
                    {
                        if(size!=0) throw exception("{%s}.%s invalid size!",**(parser->name),*name);
                        (void)vstack.push(ObjectType);
                    } break;
                        
                    case JSON_empty_array:
                    {
                        if(size!=0) throw exception("{%s}.%s invalid size!",**(parser->name),*name);
                        (void)vstack.push(ArrayType);
                    } break;
                        
                    case JSON_heavy_array:
                    {
                        if(size<=0)            throw exception("{%s}.%s invalid size!",**(parser->name),*name);
                        if(pstack.size()<size) throw exception("{%s}.%s missing values",**(parser->name),*name);
                        
                    } break;
                        
                    default:
                        throw exception("{%s}.%s is unexpected",**(parser->name),*name);
                }
                
            }
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            explicit Compiler() :
            Lang::DynamoX( Y_DYNAMOX(JSON) )
            {
            }

            virtual ~Compiler() throw()
            {
            }

            friend class singleton<Compiler>;
            static const at_exit::longevity life_time = ServerLifeTime-8;
        };


        void Load:: From( Lang::Module *module, Value & )
        {
            Compiler &json = Compiler::instance();
            json.createFrom(module);
        }

    }

}
