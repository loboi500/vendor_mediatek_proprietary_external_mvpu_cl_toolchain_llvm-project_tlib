// SPDX-License-Identifier: Apache-2.0
#ifndef MVPU_ELF_C_API_H
#define MVPU_ELF_C_API_H

#include "MVPU_ELF.h"
#include "MVPU_ELF/ELF.h"

#include "PrimeLib/Option.h"

namespace mvpu_elf { namespace c_api {

inline prime_lib::Option<DebugInfoID> toInternal(const MVPUDbgID &id) noexcept
{
    using RetTy = prime_lib::Option<DebugInfoID>;

    RetTy::ValTy internal;

    switch (id.type)
    {
    default: goto fail;
    case MVPU_PROC_HGC: internal.type = DebugInfoID::HGC; break;
    case MVPU_PROC_CU : internal.type = DebugInfoID::CU ; break;
    }

    internal.gid = id.gid;
    internal.kid = id.kid;

    return RetTy::Some(internal);

fail:
    return RetTy::None();
}

inline prime_lib::Option<MVPUDbgID> fromInternal(const DebugInfoID &internal) noexcept
{
    using RetTy = prime_lib::Option<MVPUDbgID>;

    RetTy::ValTy id;

    switch (internal.type)
    {
    default: goto fail;
    case DebugInfoID::HGC: id.type = MVPU_PROC_HGC; break;
    case DebugInfoID::CU : id.type = MVPU_PROC_CU ; break;
    }

    id.gid = internal.gid;
    id.kid = internal.kid;

    return RetTy::Some(id);

fail:
    return RetTy::None();
}

inline prime_lib::Option<SpillLoc> toInternal(const MVPUDbgSpillLoc &loc) noexcept
{
    using RetTy = prime_lib::Option<SpillLoc>;

    RetTy::ValTy internal;

    switch (loc.type)
    {
    default: goto fail;
    case MVPU_SPILL_VPU_LOAD  : internal.type = SpillLoc::VPU_LOAD; break;
    case MVPU_SPILL_VPU_STORE : internal.type = SpillLoc::VPU_STORE; break;
    case MVPU_SPILL_VCU_LOAD  : internal.type = SpillLoc::VCU_LOAD; break;
    case MVPU_SPILL_VCU_STORE : internal.type = SpillLoc::VCU_STORE; break;
    case MVPU_SPILL_RV_LOAD   : internal.type = SpillLoc::RV_LOAD; break;
    case MVPU_SPILL_RV_STORE  : internal.type = SpillLoc::RV_STORE; break;
    }

    internal.pc = loc.pc;
    internal.slot = loc.slot;

    return RetTy::Some(internal);

fail:
    return RetTy::None();
}

inline prime_lib::Option<MVPUDbgSpillLoc> fromInternal(const SpillLoc &internal) noexcept
{
    using RetTy = prime_lib::Option<MVPUDbgSpillLoc>;

    RetTy::ValTy loc;

    switch (internal.type)
    {
    default: goto fail;
    case SpillLoc::VPU_LOAD  : loc.type = MVPU_SPILL_VPU_LOAD; break;
    case SpillLoc::VPU_STORE : loc.type = MVPU_SPILL_VPU_STORE; break;
    case SpillLoc::VCU_LOAD  : loc.type = MVPU_SPILL_VCU_LOAD; break;
    case SpillLoc::VCU_STORE : loc.type = MVPU_SPILL_VCU_STORE; break;
    case SpillLoc::RV_LOAD   : loc.type = MVPU_SPILL_RV_LOAD; break;
    case SpillLoc::RV_STORE  : loc.type = MVPU_SPILL_RV_STORE; break;
    }

    loc.pc = internal.pc;
    loc.slot = internal.slot;

    return RetTy::Some(loc);

fail:
    return RetTy::None();
}

inline prime_lib::Option<SWPLoc> toInternal(const MVPUDbgSWPLoc &loc) noexcept
{
    using RetTy = prime_lib::Option<SWPLoc>;

    RetTy::ValTy internal;

    internal.pc = loc.pc;

    return RetTy::Some(internal);
}

inline prime_lib::Option<MVPUDbgSWPLoc> fromInternal(const SWPLoc &internal) noexcept
{
    using RetTy = prime_lib::Option<MVPUDbgSWPLoc>;

    RetTy::ValTy loc;

    loc.pc = internal.pc;

    return RetTy::Some(loc);
}

inline prime_lib::Option<BasicBlockRange> toInternal(const MVPUDbgBasicBlock &bb) noexcept
{
    using RetTy = prime_lib::Option<BasicBlockRange>;

    RetTy::ValTy internal;

    internal.first = bb.first;
    internal.last = bb.last;

    return RetTy::Some(internal);
}

inline prime_lib::Option<MVPUDbgBasicBlock> fromInternal(const BasicBlockRange &internal) noexcept
{
    using RetTy = prime_lib::Option<MVPUDbgBasicBlock>;

    RetTy::ValTy bb;

    bb.first = internal.first;
    bb.last = internal.last;

    return RetTy::Some(bb);
}

inline ELF * toInternal(struct MVPUELF *elf) noexcept
{
    return reinterpret_cast<ELF *>(elf);
}

inline MVPUELF * fromInternal(ELF *elf) noexcept
{
    return reinterpret_cast<MVPUELF *>(elf);
}

}} // namespace mvpu_elf::c_api

#endif // MVPU_ELF_C_API_H
