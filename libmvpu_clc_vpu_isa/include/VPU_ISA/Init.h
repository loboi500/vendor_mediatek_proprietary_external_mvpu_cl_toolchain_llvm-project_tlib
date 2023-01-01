#ifndef VPU_ISA_INIT_H
#define VPU_ISA_INIT_H

#include "ISA_API.h"

namespace vpu_isa
{

ISA_API_ENTRY
void init();

ISA_API_ENTRY
void cleanup();

} // namespace vpu_isa

#endif // ! VPU_ISA_DISASSEMBLER_H
