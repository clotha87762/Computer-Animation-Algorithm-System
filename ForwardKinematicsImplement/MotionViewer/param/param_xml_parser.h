#ifndef _PARAM_XML_PARSER_H_
#define _PARAM_XML_PARSER_H_

#include <string>
#include <memory>
#include <typeinfo>
#include "boost/noncopyable.hpp"
#include "boost/optional.hpp"
#include "param_type.h"

namespace param {

class XMLParser final : boost::noncopyable
{

public:

    typedef std::string Key_t;
    typedef boost::optional<const Ptree_t &> ConstNodeHdl_t;

    XMLParser();
    ~XMLParser();

    static Key_t AttrKey();

    void ReadFile(const std::string &file_name);

    template<typename Data>
    Data Value(const Key_t &key) const;
    ConstNodeHdl_t ChildNode(const Key_t &key) const;

protected:

private:

    std::unique_ptr<Ptree_t> ptree_;
};

} // namespace param {

//#include "param_xml_parser_src.h"

namespace param {

// public func.

template<typename Data>
Data XMLParser::Value(const Key_t &key) const
{
    return ptree_->get_optional<Data>(key).get();
}

} // namespace param {

#endif // #ifndef _PARAM_XML_PARSER_H_
