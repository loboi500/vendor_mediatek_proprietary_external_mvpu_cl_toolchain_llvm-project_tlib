// SPDX-License-Identifier: Apache-2.0

#ifndef VPU_LLVM_GPU_COMMON_LLVM_H
#define VPU_LLVM_GPU_COMMON_LLVM_H

#include "MVPU_ELF/ELF.h"

#include "PrimeLib/Result.h"
#include "PrimeLib/string_view.h"

#include "absl/types/span.h"

#include "llvm/Support/raw_ostream.h"

#include "yl_exports.h"

#include <string>
#include <unordered_map>
#include <vector>

#include <setjmp.h>
#include <stddef.h>

namespace vpu_llvm
{

// ----------------------------------------------------------------------------

void gpuInitializeLLVM(unsigned int EnableDebug, unsigned int EnableUCF, const char **CodegenOptions, size_t NumOpts, const char *Overview);
void gpuFinalizeLLVM();

#define JF_VALID_JUMPER (1 << 0)
#define JF_ASSERTION 0

int gpuHasJumper();
int gpuIsJumperValid();
void gpuJumpIfValid();
void gpuInvalidateJumper(int cond);
jmp_buf *gpuActivateJumper(unsigned int Flag, void *MsgOS);

#ifdef _WIN32
#define __SETJMP _setjmp
#else
#define __SETJMP setjmp
#endif

#define __TRY__(X, Y) \
    try \
    { \
        int __has_jumper = gpuHasJumper(); \
        int __is_jumper_valid = gpuIsJumperValid(); \
        if (__has_jumper == 0 || __is_jumper_valid != 0 || __SETJMP(*gpuActivateJumper(X, (void *)Y)) == 0)

#define __INVALIDATE_JUMPER__ \
        gpuInvalidateJumper(__has_jumper != 0 && __is_jumper_valid == 0);

#define __CATCH__(X) \
        else \
            X; \
        __INVALIDATE_JUMPER__ \
    } \
    catch (...) \
    { \
        X; \
    }

// ----------------------------------------------------------------------------

prime_lib::Result<mvpu_elf::ELF, std::string> gpuLinkELF(
    absl::Span<mvpu_elf::ELF::RawData> Objs,
    absl::string_view Entry, const mvpu_elf::SymbolInfo &SymAddrMap, bool stripAll);

// ----------------------------------------------------------------------------

inline void outputCompileMsg(llvm::raw_string_ostream &OS, absl::string_view Msg, bool Prefix = true, bool Warning = false)
{
    if (!Msg.empty())
    {
        if (Prefix)
            OS << (Warning ? "[Compile Warning] " : "[Compile Error] ");
        OS << Msg;
        if (Prefix)
            OS << "\n";
    }
}

}

#endif /* VPU_LLVM_GPU_COMMON_LLVM_H */
