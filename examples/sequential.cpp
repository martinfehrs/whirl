#include "sequential.hpp"


int main(int argc, char** argv)
{
    if (argc != 3)
    {
        if(argc < 2)
            std::cerr << "missing input file\n"; 

        if(argc < 3)
            std::cerr << "missing output file\n";

        if(argc > 3)
            std::cerr << "to many arguments\n";

        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[1]);
    std::ofstream ofs(argv[2]);

    std::cout << "output file is: " << argv[2] << '\n';

    if (!ifs.is_open())
    {
        std::cerr << "file \"" << argv[1] << "\" not found\n";
        return EXIT_FAILURE;
    }

    if (!ofs.is_open())
    {
        std::cerr << "file \"" << argv[2] << "\" not found\n";
        return EXIT_FAILURE;
    }

    LL1::code_position pos{ 1, 1 };

    try
    {
        for(auto temperature : sequential::read_data(ifs, pos))
            ofs << temperature << " ";

        ofs.close();

        return EXIT_SUCCESS;
    }
    catch(LL1::unexpected_input)
    {
        if (LL1::is_character(ifs))
        {
            std::cerr << "unexpeced token "
                << static_cast<char>(LL1::next(ifs, [](const char& c){ return c; }))
                << " at ("
                << pos.row
                << ", "
                << pos.col
                << ")\n";
        }
        else
        {
            std::cerr << "unexpected end\n";
        }

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
