#ifndef _PARAM_HOLDER_H_
#define _PARAM_HOLDER_H_

#include "param_type.h"

namespace param {

template<typename Data>
Data ConfigValue(const Key_t &key);

} // namespace param {

#endif // #ifndef _PARAM_HOLDER_HPP_
