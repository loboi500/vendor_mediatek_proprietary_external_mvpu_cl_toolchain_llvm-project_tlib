// SPDX-License-Identifier: Apache-2.0

#ifndef VPU_LLVM_CODEGEN_H
#define VPU_LLVM_CODEGEN_H

#include "CL_Compiler/Utilities.h"

#include "GPUUtil.h"
#include "yl_exports.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"

#include <memory>

namespace vpu_llvm
{

// ----------------------------------------------------------------------------

std::unique_ptr<llvm::Module> gpuGetModule(llvm::Module *M, llvm::LLVMContext &C);

using FuncToCallerOrCalleeMap = absl::flat_hash_map<llvm::Function *, absl::flat_hash_set<llvm::Function *>>;

using ExportFuncSet = absl::flat_hash_set<llvm::StringRef>;

struct ExportFuncTy
{
    enum class Kind
    {
        Kernel,
        General,
    };

    static ExportFuncTy Kernel(llvm::StringRef name)
    {
        ExportFuncSet names;
        names.insert(name);
        return ExportFuncTy(Kind::Kernel, std::move(names));
    }

    static ExportFuncTy General(ExportFuncSet names)
    {
        return ExportFuncTy(Kind::General, std::move(names));
    }

    ExportFuncTy() = delete;

    ExportFuncTy(Kind kind_, ExportFuncSet names_)
        : kind(kind_), names(std::move(names_))
    {
        assert(!names.empty());
        assert(kind != Kind::Kernel || (names.size() == 1 && !names.begin()->empty()));
    }

    Kind kind;
    ExportFuncSet names;
};

FuncToCallerOrCalleeMap gpuNewUsedFuncToCallerMap(llvm::Module *M, const ExportFuncSet &Names,
                                          llvm::TargetMachine *Target = nullptr);

void gpuRemoveUnusedFunction(llvm::Module *M,
                             const FuncToCallerOrCalleeMap &UsedFuncToCallerMap);

CompileState gpuClCollectInfo(llvm::Module *M,
                              CompileOut *Out,
                              llvm::raw_string_ostream &MsgOS,
                              std::unique_ptr<llvm::TargetMachine> &TM,
                              std::unique_ptr<llvm::legacy::FunctionPassManager> &FPM,
                              std::unique_ptr<llvm::legacy::PassManager> &PM);

CompileState gpuClCompileLib(llvm::Module *M,
                             CompileOut *Out,
                             llvm::raw_string_ostream &MsgOS,
                             std::unique_ptr<llvm::TargetMachine> &TM,
                             std::unique_ptr<llvm::legacy::FunctionPassManager> &FPM,
                             std::unique_ptr<llvm::legacy::PassManager> &PM);

CompileState gpuClCodegen(llvm::Module *M,
                          const ExportFuncTy &Exports,
                          void *Data,
                          CompileOut *Out,
                          CompileOutList *LibBinCOL,
                          llvm::raw_string_ostream &MsgOS,
                          std::unique_ptr<llvm::TargetMachine> &TM,
                          std::unique_ptr<llvm::legacy::FunctionPassManager> &FPM,
                          std::unique_ptr<llvm::legacy::PassManager> &PM);

} // namespace vpu_llvm

#endif /* VPU_LLVM_CODEGEN_H */
