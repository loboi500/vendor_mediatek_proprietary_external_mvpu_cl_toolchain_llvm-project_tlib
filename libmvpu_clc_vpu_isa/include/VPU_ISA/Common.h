#ifndef VPU_ISA_COMMON_H
#define VPU_ISA_COMMON_H

#include "absl/types/span.h"

#include <vector>

namespace vpu_isa
{
using Bytes = std::vector<unsigned char>;

using BytesRef = absl::Span<const unsigned char>;

} // namespace vpu_isa

#endif // ! VPU_ISA_COMMON_H
