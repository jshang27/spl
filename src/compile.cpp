#include <fstream>

#include "compile.hpp"
#include "error.hpp"

int spl::compile_to_asm(std::string ofn, std::shared_ptr<spl::token> first)
{
    {
        std::ofstream ofs(ofn);
        if (ofs.fail())
        {
            spl::error("could not open " + ofn);
            return spl::FAIL;
        }
        ofs << "Hello world!\n";
    }
    return spl::SUCCESS;
}