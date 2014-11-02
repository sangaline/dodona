#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <string>
#include <fstream>

namespace bar = boost::archive;
namespace bio = boost::iostreams;

template <typename T> inline std::string SaveGZString(const T & o) {
        std::ostringstream oss;
        { 
                bio::filtering_stream<bio::output> f;
                f.push(bio::gzip_compressor());
                f.push(oss);
                bar::binary_oarchive oa(f);
                oa << o;
        } // gzip_compressor flushes when f goes out of scope
        return oss.str();
}

template <typename T> inline void LoadGZString(T & o, const std::string& s) {
        std::istringstream iss(s);
        bio::filtering_stream<bio::input> f;
        f.push(bio::gzip_decompressor());
        f.push(iss);
        bar::binary_iarchive ia(f);
        ia >> o;
}

template<typename T> void SaveToFile(T &o, const std::string& filename) {
    std::ofstream f;
    f.open (filename.c_str(), std::ios::binary);
    f << SaveGZString(o);
    f.close();
}

template<typename T> void LoadFromFile(T &o, const std::string& filename) {
    std::ifstream f;
    f.open (filename.c_str(), std::ios::binary);
    if(!f.good()) {
        std::string error_message = "File \"" + filename + "\" either does not exist or cannot be read.";
        throw(error_message);
    }
    std::string s;
    f.seekg(0, std::ios::end);
    s.resize(f.tellg());
    f.seekg(0, std::ios::beg);
    f.read(&s[0], s.size());
    f.close();
    LoadGZString(o, s);
}
