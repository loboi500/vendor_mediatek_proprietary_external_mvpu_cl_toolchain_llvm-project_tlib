#ifndef VPU_ISA_ASSEMBLER_H
#define VPU_ISA_ASSEMBLER_H

#include "VPU_ISA/Common.h"

#include "ISA_API.h"

#include "PrimeLib/Option.h"
#include "PrimeLib/Result.h"
#include "PrimeLib/string_view.h"

#include <memory>
#include <string>
#include <vector>

namespace vpu_isa
{

class ISA_API_ENTRY Assembler
{
public:
    static prime_lib::Option<Assembler> make();

    Assembler(const Assembler &) = delete;

    Assembler(Assembler &&);

    ~Assembler();

    Assembler & operator=(const Assembler &) = delete;

    Assembler & operator=(Assembler &&);

    prime_lib::Result<Bytes, std::string> encode(absl::string_view input) const;

private:
    struct Impl;

    Assembler();

    std::unique_ptr<Impl> impl;
}; // class Assembler

} // namespace vpu_isa

#endif // ! VPU_ISA_ASSEMBLER_H
