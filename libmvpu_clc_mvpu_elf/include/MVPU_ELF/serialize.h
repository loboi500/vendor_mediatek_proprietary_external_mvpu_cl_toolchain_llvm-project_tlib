// SPDX-License-Identifier: Apache-2.0
#ifndef MVPU_ELF_SERIALIZE_H
#define MVPU_ELF_SERIALIZE_H

#include "MVPU_ELF/ELF.h"

#include "PrimeLib/Serialize.h"
#include "PrimeLib/Try.h"

#include "absl/meta/type_traits.h"

namespace prime_lib { namespace serialize {

#define ENCODE_V2(t, v, e) Encodable<t>::encode(v, e)
#define ENCODE_V(v, e) ENCODE_V2(absl::decay_t<decltype(v)>, v, e)

#define DECODE_T(t, d) Decodable<t>::decode(d)
#define DECODE_V(v, d) DECODE_T(absl::decay_t<decltype(v)>, d)

// DebugInfoID::Ty
template <>
struct Encodable<::mvpu_elf::DebugInfoID::Ty>
{
    using ValTy = ::mvpu_elf::DebugInfoID::Ty;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(ValTy ty, E &e)
    {
        auto val = static_cast<unsigned char>(ty);
        PRIME_LIB_TRYV(ENCODE_V(val, e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::DebugInfoID::Ty>
{
    using ValTy = ::mvpu_elf::DebugInfoID::Ty;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        PRIME_LIB_TRY(type, DECODE_T(unsigned char, d));

        return RetTy::Ok(static_cast<ValTy>(type));
    }
};

// DebugInfoID
template <>
struct Encodable<::mvpu_elf::DebugInfoID>
{
    using ValTy = ::mvpu_elf::DebugInfoID;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(const ValTy &id, E &e)
    {
        PRIME_LIB_TRYV(ENCODE_V(id.type, e));
        PRIME_LIB_TRYV(ENCODE_V(id.gid, e));
        PRIME_LIB_TRYV(ENCODE_V(id.kid, e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::DebugInfoID>
{
    using ValTy = ::mvpu_elf::DebugInfoID;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ValTy id;

        PRIME_LIB_TRY(type, DECODE_V(id.type, d));
        PRIME_LIB_TRY(gid , DECODE_V(id.gid, d));
        PRIME_LIB_TRY(kid , DECODE_V(id.kid, d));

        id.type = type;
        id.gid = gid;
        id.kid = kid;

        return RetTy::Ok(id);
    }
};

// SpillLoc::Ty
template <>
struct Encodable<::mvpu_elf::SpillLoc::Ty>
{
    using ValTy = ::mvpu_elf::SpillLoc::Ty;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(ValTy ty, E &e)
    {
        auto val = static_cast<unsigned char>(ty);
        PRIME_LIB_TRYV(ENCODE_V(val, e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::SpillLoc::Ty>
{
    using ValTy = ::mvpu_elf::SpillLoc::Ty;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        PRIME_LIB_TRY(type, DECODE_T(unsigned char, d));

        return RetTy::Ok(static_cast<ValTy>(type));
    }
};

// SpillLoc
template <>
struct Encodable<::mvpu_elf::SpillLoc>
{
    using ValTy = ::mvpu_elf::SpillLoc;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(const ValTy &loc, E &e)
    {
        PRIME_LIB_TRYV(ENCODE_V(loc.pc, e));
        PRIME_LIB_TRYV(ENCODE_V(loc.slot, e));
        PRIME_LIB_TRYV(ENCODE_V(loc.type, e));
        PRIME_LIB_TRYV(ENCODE_V(loc.spillvar, e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::SpillLoc>
{
    using ValTy = ::mvpu_elf::SpillLoc;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ValTy loc;

        PRIME_LIB_TRY(pc  , DECODE_V(loc.pc, d));
        PRIME_LIB_TRY(slot, DECODE_V(loc.slot, d));
        PRIME_LIB_TRY(type, DECODE_V(loc.type, d));
        PRIME_LIB_TRY(var, DECODE_V(loc.spillvar, d));

        loc.pc = pc;
        loc.slot = slot;
        loc.type = type;
        loc.spillvar = std::move(var);

        return RetTy::Ok(loc);
    }
};

// SpillInfo
template <>
struct Encodable<::mvpu_elf::SpillInfo>
{
    using ValTy = ::mvpu_elf::SpillInfo;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(const ValTy &info, E &e)
    {
        for (auto &loc : info.inner)
        {
            PRIME_LIB_TRYV(ENCODE_V(loc, e));
        }
        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::SpillInfo>
{
    using ValTy = ::mvpu_elf::SpillInfo;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ValTy info;

        while (true)
        {
            if (d.isEOF())
                break;
            PRIME_LIB_TRY(loc, DECODE_T(::mvpu_elf::SpillLoc, d));
            info.inner.push_back(std::move(loc));
        }
        return RetTy::Ok(info);
    }
};

// SWPLoc
template <>
struct Encodable<::mvpu_elf::SWPLoc>
{
    using ValTy = ::mvpu_elf::SWPLoc;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(const ValTy &loc, E &e)
    {
        PRIME_LIB_TRYV(ENCODE_V(loc.pc, e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::SWPLoc>
{
    using ValTy = ::mvpu_elf::SWPLoc;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ValTy loc;

        PRIME_LIB_TRY(pc  , DECODE_V(loc.pc, d));

        loc.pc = pc;

        return RetTy::Ok(loc);
    }
};

// SWPInfo
template <>
struct Encodable<::mvpu_elf::SWPInfo>
{
    using ValTy = ::mvpu_elf::SWPInfo;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(const ValTy &info, E &e)
    {
        for (auto &loc : info.inner)
        {
            PRIME_LIB_TRYV(ENCODE_V(loc, e));
        }
        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::SWPInfo>
{
    using ValTy = ::mvpu_elf::SWPInfo;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ValTy info;

        while (true)
        {
            if (d.isEOF())
                break;
            PRIME_LIB_TRY(loc, DECODE_T(::mvpu_elf::SWPLoc, d));
            info.inner.push_back(std::move(loc));
        }
        return RetTy::Ok(info);
    }
};

// BasicBlockRange
template <>
struct Encodable<::mvpu_elf::BasicBlockRange>
{
    using ValTy = ::mvpu_elf::BasicBlockRange;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(const ValTy &bb, E &e)
    {
        PRIME_LIB_TRYV(ENCODE_V(bb.first, e));
        PRIME_LIB_TRYV(ENCODE_V(bb.last, e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::BasicBlockRange>
{
    using ValTy = ::mvpu_elf::BasicBlockRange;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ValTy bb;

        PRIME_LIB_TRY(first, DECODE_V(bb.first, d));
        PRIME_LIB_TRY(last , DECODE_V(bb.last, d));

        bb.first = first;
        bb.last = last;

        return RetTy::Ok(bb);
    }
};

// ControlFlowGraph
template <>
struct Encodable<::mvpu_elf::ControlFlowGraph>
{
    using ValTy = ::mvpu_elf::ControlFlowGraph;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(const ValTy &cfg, E &e)
    {
        for (auto &edge : cfg.inner)
        {
            PRIME_LIB_TRYV(ENCODE_V(std::get<0>(edge), e));
            PRIME_LIB_TRYV(ENCODE_V(std::get<1>(edge), e));
        }
        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::ControlFlowGraph>
{
    using ValTy = ::mvpu_elf::ControlFlowGraph;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ValTy cfg;

        while (true)
        {
            if (d.isEOF())
                break;

            PRIME_LIB_TRY(from, DECODE_T(::mvpu_elf::BasicBlockRange, d));
            PRIME_LIB_TRY(to, DECODE_T(::mvpu_elf::BasicBlockRange, d));

            cfg.inner.emplace_back(std::move(from), std::move(to));
        }
        return RetTy::Ok(cfg);
    }
};

// SymbolAddr::Space
template <>
struct Encodable<::mvpu_elf::SymbolAddr::Space>
{
    using ValTy = ::mvpu_elf::SymbolAddr::Space;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(ValTy ty, E &e)
    {
        auto val = static_cast<unsigned char>(ty);
        PRIME_LIB_TRYV(ENCODE_V(val, e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::SymbolAddr::Space>
{
    using ValTy = ::mvpu_elf::SymbolAddr::Space;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        PRIME_LIB_TRY(type, DECODE_T(unsigned char, d));

        return RetTy::Ok(static_cast<ValTy>(type));
    }
};

// SymbolAddr
template <>
struct Encodable<::mvpu_elf::SymbolAddr>
{
    using ValTy = ::mvpu_elf::SymbolAddr;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(const ValTy &sym, E &e)
    {
        PRIME_LIB_TRYV(ENCODE_V(sym.space, e));
        PRIME_LIB_TRYV(ENCODE_V(sym.addr, e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::SymbolAddr>
{
    using ValTy = ::mvpu_elf::SymbolAddr;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ValTy sym;

        PRIME_LIB_TRY(space, DECODE_V(sym.space, d));
        PRIME_LIB_TRY(addr , DECODE_V(sym.addr, d));

        sym.space = space;
        sym.addr = addr;

        return RetTy::Ok(sym);
    }
};

// LatencyTable
template <>
struct Encodable<::mvpu_elf::LatencyTable>
{
    using ValTy = ::mvpu_elf::LatencyTable;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(ValTy table, E &e)
    {
        auto val = static_cast<unsigned char>(table);
        PRIME_LIB_TRYV(ENCODE_V(val, e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::LatencyTable>
{
    using ValTy = ::mvpu_elf::LatencyTable;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        PRIME_LIB_TRY(table, DECODE_T(unsigned char, d));

        return RetTy::Ok(static_cast<ValTy>(table));
    }
};

// LatencyInfoLoc
template <>
struct Encodable<::mvpu_elf::LatencyInfoLoc>
{
    using ValTy = ::mvpu_elf::LatencyInfoLoc;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(const ValTy &info, E &e)
    {
        PRIME_LIB_TRYV(ENCODE_V(info.loc.pc, e));
        PRIME_LIB_TRYV(ENCODE_V(info.loc.slot, e));
        PRIME_LIB_TRYV(ENCODE_V(info.info.table, e));
        PRIME_LIB_TRYV(ENCODE_V(info.info.producerIdx, e));
        PRIME_LIB_TRYV(ENCODE_V(info.info.consumerIdx, e));
        PRIME_LIB_TRYV(ENCODE_V(info.info.latency, e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::LatencyInfoLoc>
{
    using ValTy = ::mvpu_elf::LatencyInfoLoc;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ValTy info;

        PRIME_LIB_TRY(pc, DECODE_V(info.loc.pc, d));
        PRIME_LIB_TRY(slot, DECODE_V(info.loc.slot, d));
        PRIME_LIB_TRY(table, DECODE_V(info.info.table, d));
        PRIME_LIB_TRY(producerIdx, DECODE_V(info.info.producerIdx, d));
        PRIME_LIB_TRY(consumerIdx, DECODE_V(info.info.consumerIdx, d));
        PRIME_LIB_TRY(latency, DECODE_V(info.info.latency, d));

        info.loc.pc = pc;
        info.loc.slot = slot;
        info.info.table = table;
        info.info.producerIdx = producerIdx;
        info.info.consumerIdx = consumerIdx;
        info.info.latency = latency;

        return RetTy::Ok(info);
    }
};

// LatencyInfo
template <>
struct Encodable<::mvpu_elf::LatencyInfo>
{
    using ValTy = ::mvpu_elf::LatencyInfo;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(const ValTy &info, E &e)
    {
        for (auto &loc : info.inner)
        {
            PRIME_LIB_TRYV(ENCODE_V(loc, e));
        }
        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::LatencyInfo>
{
    using ValTy = ::mvpu_elf::LatencyInfo;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ValTy info;

        while (true)
        {
            if (d.isEOF())
                break;
            PRIME_LIB_TRY(loc, DECODE_T(::mvpu_elf::LatencyInfoLoc, d));
            info.inner.push_back(std::move(loc));
        }
        return RetTy::Ok(info);
    }
};

// ELF
template <>
struct Encodable<::mvpu_elf::ELF>
{
    using ValTy = ::mvpu_elf::ELF;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(ValTy &elf, E &e)
    {
        using ErrTy = typename E::ErrTy;

        auto binary = elf.save();
        if (!binary.hasVal())
            return RetTy::Err(ErrTy{});

        PRIME_LIB_TRYV(ENCODE_V(binary.getVal(), e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_elf::ELF>
{
    using ValTy = ::mvpu_elf::ELF;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        using ErrTy = typename D::ErrTy;

        PRIME_LIB_TRY(binary, DECODE_T(std::vector<unsigned char>, d));

        auto elf = ValTy::load(binary);
        if (!elf.hasVal())
            return RetTy::Err(ErrTy{});

        return RetTy::Ok(std::move(elf.getVal()));
    }
};

#undef DECODE_V
#undef DECODE_T

#undef ENCODE_V
#undef ENCODE_V2

}} // namespace prime_lib::serialize

#endif // MTKVPU_ELF_SERIALIZE_H
