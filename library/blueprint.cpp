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
        throw std::runtime_error("'destroy' received a nullptr");
    }

    delete blueprint;
}

extern "C" bool verify(
    Blueprint::Schema *blueprint, const char *schema, const char *data)
{
    if (blueprint == nullptr) {
        throw std::runtime_error("'verify' received a nullptr");
    }

    return blueprint->verify(schema, data);
}

extern "C" const char *error(Blueprint::Schema *blueprint)
{
    if (blueprint == nullptr) {
        throw std::runtime_error("'error' received a nullptr");
    }

    return blueprint->getError().c_str();
}
