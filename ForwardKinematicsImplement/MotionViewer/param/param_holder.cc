#include "param_holder.h"
#include "param_config.h"

namespace param {

template<typename Data>
Data ConfigValue(const Key_t &key)
{
    return Config::instance().Value<Data>(key);
}

template double ConfigValue(const Key_t &);
template int ConfigValue(const Key_t &);
template unsigned char ConfigValue(const Key_t &);
template bool ConfigValue(const Key_t &);
template std::string ConfigValue(const Key_t &);

} // namespace param {
