#include "y/program.hpp"
#include "y/graphic/color/rgb.hpp"
#include "y/associative/set.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"
#include "y/string/convert.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Graphic;

namespace {

	class Color
	{
	public:
		const uint8_t index;
		const rgb     color;

		const uint8_t & key() const throw()
		{
			return index;
		}

		inline Color(const uint8_t i, const uint8_t r, const uint8_t g, const uint8_t b) throw() :
			index(i),
			color(r, g, b)
		{
		}

		inline ~Color() throw()
		{
		}

		inline Color(const Color &_) throw() : index(_.index), color(_.color) {}



	private:
		Y_DISABLE_ASSIGN(Color);
	};

	typedef set<uint8_t, Color> ColorDB;

	class Colors : public ColorDB
	{
	public:
		vector<string> strings;

		inline Colors() : ColorDB(256, as_capacity), strings(32, as_capacity)
		{
		}


		inline virtual ~Colors() throw()
		{
		}

		void load(const string &line)
		{
			strings.free();
			tokenizer<char>::split_with(strings, line, " \t");
			const size_t nstr = strings.size();
			if (0 != (nstr % 4))
			{
				throw exception("must have a multiple of 4 bytes");
			}
			const size_t ncol = nstr / 4;
			for (size_t j = 0, i = 1; i <= ncol; ++i)
			{
				const size_t I = string_convert::to<size_t>(strings[++j]);
				if (I > 255) throw exception("invalid index");

				const size_t R = string_convert::to<size_t>(strings[++j]);
				if (R > 255) throw exception("invalid red");

				const size_t G = string_convert::to<size_t>(strings[++j]);
				if (G > 255) throw exception("invalid green");

				const size_t B = string_convert::to<size_t>(strings[++j]);
				if (B > 255) throw exception("invalid blue");

				const Color C(
					(uint8_t)I,
					(uint8_t)R,
					(uint8_t)G,
					(uint8_t)B );

				if (!insert(C))
				{
					throw exception("Multiple Color Index");
				}
			}
		}

		void loadFrom(const string &filename)
		{
			ios::icstream inp(filename);
			string line;
			while (inp.gets(line))
			{
				load(line);
			}
			sort_keys(comparison::increasing<uint8_t>);
			std::cerr << "#loaded: " << size() << std::endl;
		}


	private:
		Y_DISABLE_COPY_AND_ASSIGN(Colors);
	};


}

#include "y/ios/ocstream.hpp"

Y_PROGRAM_START()
{
	for (int iarg = 1; iarg < argc; ++iarg)
	{
		Colors        colors;
		const string  rampName = argv[iarg];
		colors.loadFrom(rampName);


		const string baseName = vfs::get_base_name(rampName);     std::cerr << "baseName  =[" << baseName << "]" << std::endl;
		const string rootName = vfs::without_extension(baseName); std::cerr << "rootName  =[" << rootName << "]" << std::endl;
		string headerName = rootName + ".hpp";                    std::cerr << "headerName=[" << headerName << "]" << std::endl;
		string sourceName = rootName + ".cpp";                    std::cerr << "sourceName=[" << sourceName << "]" << std::endl;
		string className = vfs::class_name_for(rootName);
		string guardName = vfs::cpp_label_from(rootName) + "_INCLUDED";

		std::cerr << "className=" << className << std::endl;

        exit(0);
        
		{
			ios::ocstream header(headerName);
			header << "//! \\file\n";
			header << "#ifndef " << guardName << "\n";
			header << "#define " << guardName << " 1\n\n";
			header << "#include \"y/graphic/color/ramp.hpp\"\n\n";
			header << "namespace upsylon { namespace Graphic {\n\n";

			header << "class " << className << " : public Ramp {\n";
			header << " public: explicit " << className << "() throw();\n";
			header << "         virtual ~" << className << "() throw();\n";
			header << " private: Y_DISABLE_COPY_AND_ASSIGN(" << className << ");\n";
			header << "  virtual const rgb *_256colors() const throw();\n";
			header << "};\n\n";

			header << "}}\n";
			header << "#endif\n";
		}

		{
			ios::ocstream source(sourceName);
			source << "#include \"" << headerName << "\"\n\n";
			source << "namespace upsylon { namespace Graphic {\n\n";

			source << " " << className << ":: " << className << "() throw() : Ramp() {}\n";
			source << " " << className << "::~" << className << "() throw() {}\n";

			source << " const rgb * " << className << ":: _256colors() const throw() {\n";
			source << "  static const rgb _[256] = {\n\t";
			for (unsigned i = 0; i < 256; ++i)
			{
				const Color *p = colors.search(i);
				if (!p) throw exception("Missing color %u", i);
				source("rgb(0x%02x,0x%02x,0x%02x)", unsigned(p->color.r), unsigned(p->color.g), unsigned(p->color.b));
				if (i < 255)
				{
					source << ',';
					if ((1 + i) % 8 == 0)
					{
						source << '\n' << '\t';
					}
					else
					{
						source << ' ';
					}
				}
			}

			source << "  };\n";
			source << " return _; }\n";

			source << "}}\n";

		}


	}
}
Y_PROGRAM_END()

