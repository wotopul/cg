#pragma once

#include <boost/filesystem.hpp>
#include <string>
#include <stdexcept>

namespace util {

    template<class T>
    generate_files(std::string const& root,
                   std::function<bool(std::ofstream&, int)> generator_function,
                   int count,
                   std::string const& prefix = "")
    {
        using namespace boost::filesystem;
        path p(root);
        if (!is_directory(p)) {
            throw std::invalid_argument("Given path " + p + " is not a directory");
        }

        for (int i = 1; i <= count; ++i)
        {
            path file = p / (prefix + std::to_string(i));
            std::ofstream ofs(p.string());
            if (!generator_function(ofs, i)) {
                std::cerr << "Error generating file " << file;
            }
        }
    }

}
