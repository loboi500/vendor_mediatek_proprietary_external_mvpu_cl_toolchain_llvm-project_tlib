#ifndef MVPU_DEBUG_INFO_C_API_H
#define MVPU_DEBUG_INFO_C_API_H

#include "MVPU_DebugInfo.h"
#include "MVPU_DebugInfo/DebugInfoList.h"

#include "MVPU_ELF/C_API.h"

#include "PrimeLib/Option.h"

namespace mvpu_debuginfo { namespace c_api {

inline DebugInfoList * toInternal(MVPUDbgList *list) noexcept
{
    return reinterpret_cast<DebugInfoList *>(list);
}

inline MVPUDbgList * fromInternal(DebugInfoList *internal) noexcept
{
    return reinterpret_cast<struct MVPUDbgList *>(internal);
}

inline DebugInfoList::Range * toInternal(MVPUDbgIter *iter) noexcept
{
    return reinterpret_cast<DebugInfoList::Range *>(iter);
}

inline MVPUDbgIter * fromInternal(DebugInfoList::Range *internal) noexcept
{
    return reinterpret_cast<struct MVPUDbgIter *>(internal);
}

inline prime_lib::Option<DebugInfoNewID> toInternal(const MVPUDbgNewID &newID) noexcept
{
    using RetTy = prime_lib::Option<DebugInfoNewID>;

    DebugInfoNewID internal;

    auto oldOpt = mvpu_elf::c_api::toInternal(newID.old);
    if (!oldOpt.hasVal())
        return RetTy::None();

    auto newOpt = mvpu_elf::c_api::toInternal(newID.new_);
    if (!newOpt.hasVal())
        return RetTy::None();

    internal.old = oldOpt.getVal();
    internal.new_ = newOpt.getVal();
    return RetTy::Some(internal);
}

}} // namespace mvpu_debuginfo::c_api

#endif // MVPU_DEBUG_INFO_C_API_H
