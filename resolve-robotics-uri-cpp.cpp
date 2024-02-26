#include <ResolveRoboticsURICpp.h>

#include <cstdlib>
#include <iostream>
#include <optional>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "resolve-robotics-uri-cpp: Wrong number of parameters. Usage " << argv[0] << " package://uri/of/the/model/to/resolve " << std::endl;
        return EXIT_FAILURE;
    }

    std::string errorMessage;
    auto absolute_file_name = ResolveRoboticsURICpp::resolveRoboticsURI(argv[1], errorMessage);

    if (absolute_file_name.has_value())
    {
        std::cout << absolute_file_name.value();
    }
    else 
    {
        std::cerr << "resolve-robotics-uri-cpp: Impossible to find URI " 
                  << argv[1] << ", " << errorMessage << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
