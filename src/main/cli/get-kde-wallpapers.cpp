#include "y/program.hpp"
#include "y/fs/local/fs.hpp"
#include "y/lang//pattern/matching.hpp"
#include "y/sequence/vector.hpp"
#include "y/type/point2d.hpp"
#include "y/lang/token-convert.hpp"
#include "y/fs/disk/file.hpp"

using namespace         upsylon;
typedef point2d<unit_t> sizes_t;

static inline sizes_t extract_sizes( const string &imageName )
{
    Lang::Matching      isNumber = "[:digit:]+";
    vector<Lang::Token> numbers;
    const size_t        count = isNumber.find(numbers,imageName);
    if(2!=count) throw exception("expecting WxH, got %u numbers", unsigned(count));
    return sizes_t( Lang::TokenConvert::ToUnit(numbers[1],"W"), Lang::TokenConvert::ToUnit(numbers[2],"H") );
}

Y_PROGRAM_START()
{
    string  dimensions = "1920x1080";
    if(argc>1)
    {
        dimensions = argv[1];
    }
    sizes_t desired    = extract_sizes(dimensions);
    std::cerr << "desired=" << desired << std::endl;

    vfs & fs = local_fs::instance();

    Lang::MatchString       match = "png|jpe?g";
    const string            root  = "/opt/local/share/wallpapers";
    const string            place = "/contents/images";
    auto_ptr<vfs::scanner>  scan  = fs.new_scanner(root);
    vector<string>          images(16,as_capacity);

    for(const vfs::entry *ep = scan->next(); ep; ep=scan->next() )
    {
        if(!ep->is_subdir())
        {
            continue;
        }
        const string baseName = ep->base_name;
        std::cerr << "+[" << baseName << "]" << std::endl;

        const string           imgDir = ep->path + place;
        auto_ptr<vfs::scanner> sub    = fs.new_scanner(imgDir);

        images.free();
        for(const vfs::entry *img = sub->next(); img; img=sub->next() )
        {
            if(!img->is_regular()) continue;
            if(!img->extension)    continue;
            const string ext = img->extension;
            if( !match(ext) ) continue;
            const string id = img->base_name;
            images.push_back(id);
        }
        std::cerr << " |_" << images << std::endl;
        if(images.size() <= 0 )
        {
            continue;
        }

        unit_t d2 = (desired - extract_sizes(images[1]) ).norm2();
        size_t im = 1;
        for(size_t i=2;i<=images.size();++i)
        {
            const unit_t d2tmp = (desired-extract_sizes(images[i])).norm2();
            if(d2tmp<d2)
            {
                im = i;
                d2 = d2tmp;
            }
        }
        std::cerr << " |_selecting " << images[im] << std::endl;

        const string source = root + '/' + baseName + place + '/' + images[im];
        const string target = baseName + images[im];
        std::cerr << " |_copy " << source << " to " << target << std::endl;
        ios::disk_file::copy(target,source,false);

    }

}
Y_PROGRAM_END()


