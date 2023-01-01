// SPDX-License-Identifier: Apache-2.0
#ifndef CL_COMPILER_INTERNAL_DISASSEMBLER_H
#define CL_COMPILER_INTERNAL_DISASSEMBLER_H

#include "yl_exports.h"
#include "MVPU_ELF/ELF.h"

#include "absl/types/variant.h"

#include <cstdio>

namespace cl_compiler
{
namespace internal
{

using InputELF = absl::variant<mvpu_elf::ELF *, mvpu_elf::ELF::RawData>;

YL_API_ENTRY
bool printInstr(FILE *out, const InputELF &elf, bool showPC);

} // namespace internal
} // namespace cl_compiler
#endif
