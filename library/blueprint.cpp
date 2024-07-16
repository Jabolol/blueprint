#include "JSON/Parser.hpp"

extern "C" const char *version(void)
{
    return "0.0.2";
}

extern "C" Blueprint::JSON::Parser *create_parser()
{
    return new Blueprint::JSON::Parser();
}

extern "C" void destroy_parser(Blueprint::JSON::Parser *parser)
{
    delete parser;
}

extern "C" bool parse(
    Blueprint::JSON::Parser *parser, const char *schema, const char *data)
{
    return true;
}
