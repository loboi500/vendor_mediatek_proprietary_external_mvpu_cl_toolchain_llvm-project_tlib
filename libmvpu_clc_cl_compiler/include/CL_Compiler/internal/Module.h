#ifndef CL_COMPILER_INTERNAL_MODULE_H
#define CL_COMPILER_INTERNAL_MODULE_H

#include "yl_exports.h"

#include "PrimeLib/Result.h"
#include "PrimeLib/Unit.h"

#include <string>
#include <system_error>

namespace cl_compiler
{
namespace internal
{
YL_API_ENTRY
prime_lib::Result<prime_lib::Unit, std::error_code>
    dumpSpirToTextFile(const std::string &OutFile, const void *M);

} // namespace internal
} // namespace cl_compiler
#endif
