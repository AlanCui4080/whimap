#include "parser.hpp"
using namespace whimap;

parser::parser(std::stringstream&& str)
    : input(std::move(str))
{
    
}
