#include "param_xml_parser.h"
#include <fstream>
#include "console_log.h"

namespace param {

// public func.

XMLParser::XMLParser()
    :ptree_(new Ptree_t)
{
}

XMLParser::~XMLParser()
{
}

XMLParser::Key_t XMLParser::AttrKey()
{
    static const Key_t kAttrKey("<xmlattr>.");
    return kAttrKey;
}

void XMLParser::ReadFile(const std::string &file_name)
{
    std::ifstream input_stream(file_name.c_str(), std::ifstream::in);

    if (!input_stream.good())
    {
        LOGERR << "Failed to open " << file_name << std::endl;
        assert(FALSE);
    }

    boost::property_tree::read_xml(
            input_stream,
            *ptree_
            );
}

XMLParser::ConstNodeHdl_t XMLParser::ChildNode(const Key_t &key) const
{
    return ConstNodeHdl_t(ptree_->get_child(key));
}

// protected func.

// private func.

} // namespace param {
