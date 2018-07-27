#ifndef _PARAM_CONFIG_H_
#define _PARAM_CONFIG_H_

#include <memory>
#include "boost/noncopyable.hpp"
#include "param_type.h"

namespace param {

class XMLParser;

class Config final : boost::noncopyable
{

public:

    static Config &instance();

    virtual ~Config();

    template<typename Data>
    Data Value(const Key_t &key) const;

protected:

private:

    Config();

    std::unique_ptr<XMLParser> parser_;
};

} // namespace param {

#endif // #ifndef _PARAM_CONFIG_H_
