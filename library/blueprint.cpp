#include "JSON/Parser.hpp"

extern "C" const char *version(void)
{
    return "0.0.2";
}

extern "C" void *create_parser()
{
    Blueprint::JSON::Parser *parser = new Blueprint::JSON::Parser();

    return parser;
}

extern "C" void destroy_parser(void *parser)
{
    delete static_cast<Blueprint::JSON::Parser *>(parser);
}
