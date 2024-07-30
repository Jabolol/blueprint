#include "Schema.hpp"

extern "C" const char *version(void)
{
    return "0.0.2";
}

extern "C" Blueprint::Schema *create(void)
{
    return new (std::nothrow) Blueprint::Schema();
}

extern "C" void destroy(Blueprint::Schema *blueprint)
{
    if (blueprint == nullptr) {
        return;
    }

    delete blueprint;
}

extern "C" bool verify(
    Blueprint::Schema *blueprint, const char *schema, const char *data)
{
    if (blueprint == nullptr) {
        return false;
    }

    return blueprint->verify(schema, data);
}

extern "C" const char *error(Blueprint::Schema *blueprint)
{
    if (blueprint == nullptr) {
        return "'error' received a nullptr";
    }

    return blueprint->getError().c_str();
}
