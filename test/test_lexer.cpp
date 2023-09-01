#include "parser.hpp"
#include <stdio.h>
#include <iostream>
int main()
{
    auto fp = fopen(__TEST_LEXER_INPUT_PATH, "r");
    perror("fopen");
    size_t count = 0;
    auto   par = whimap::parser(fp);
    for (const auto& i : par.lexdata())
    {
        std::cout << i << " ";
        if (i != count)
        {
            std::cout << std::endl
                      << "Unmatched Excepted:" << count << std::endl;
            exit(EXIT_FAILURE);
        }
        count++;
    }
}
