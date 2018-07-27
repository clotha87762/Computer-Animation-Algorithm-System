#include "param_config.h"
#include "param_xml_parser.h"
#include "param_config_key.h"

namespace param {

// public func.

Config &Config::instance()
{
    static Config config_instance;
    return config_instance;
}

Config::~Config()
{
}

template<typename Data>
Data Config::Value(const Key_t &key) const
{
    return parser_->Value<Data>(config_key::ParentPath + key);
}

template double Config::Value<double>(const Key_t &) const;
template int Config::Value<int>(const Key_t &) const;
template unsigned char Config::Value<unsigned char>(const Key_t &) const;
template bool Config::Value<bool>(const Key_t &) const;
template std::string Config::Value<std::string>(const Key_t &) const;

// protected func.

// private func.

Config::Config()
    :parser_(new XMLParser)
{
    parser_->ReadFile("./parameter/config.xml");
}

} // namespace param {
