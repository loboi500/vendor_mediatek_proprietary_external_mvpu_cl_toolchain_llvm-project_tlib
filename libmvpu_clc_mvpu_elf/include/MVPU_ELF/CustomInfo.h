// SPDX-License-Identifier: Apache-2.0
#ifndef MVPU_ELF_CUSTOM_INFO_H
#define MVPU_ELF_CUSTOM_INFO_H

#include "MVPU_ELF.h"

#include "absl/hash/hash.h"
#include "absl/strings/string_view.h"
#include "absl/utility/utility.h"

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace mvpu_elf
{

// debug id
struct DebugInfoID
{
    enum Ty : std::uint8_t
    {
        HGC = 0,
        CU,
    } type;

    std::uint16_t gid;
    std::uint16_t kid;

    MVPU_ELF_ENTRY
    std::size_t hash() const;

    friend bool operator==(const DebugInfoID &a, const DebugInfoID &b)
    {
        return    a.type == b.type
               && a.gid == b.gid
               && a.kid == b.kid;
    }

    template <typename H>
    friend H AbslHashValue(H state, const DebugInfoID& id)
    {
        return H::combine(absl::move(state), id.type, id.gid, id.kid);
    }
};

const char * const DBG_ID_SECTION_NAME = ".mvpu.dbg.id";

// spill info
struct SpillLoc
{
    std::uint32_t pc;
    std::uint8_t slot;

    enum Ty : std::uint8_t
    {
        VPU_LOAD,
        VPU_STORE,
        VCU_LOAD,
        VCU_STORE,
        RV_LOAD,
        RV_STORE,
    } type;

    std::string spillvar;

    friend bool operator<(const SpillLoc &a, const SpillLoc &b)
    {
        return a.pc < b.pc || (!(b.pc < a.pc) && a.slot < b.slot);
    }
};

const char * const SPILL_INFO_SECTION_NAME = ".mvpu.dbg.spill";

struct SpillInfo
{
    std::vector<SpillLoc> inner;
};

// swp info
struct SWPLoc
{
    std::uint32_t pc;

    friend bool operator<(const SWPLoc &a, const SWPLoc &b)
    {
        return a.pc < b.pc;
    }
};

const char * const SWP_INFO_SECTION_NAME = ".mvpu.dbg.swp";

struct SWPInfo
{
    std::vector<SWPLoc> inner;
};

// latency info
enum class DependencyKind : std::uint8_t
{
    DK_First = 0,
    DK_RAW = DK_First,
    DK_RAR,
    DK_WAR,
    DK_WAW,
    DK_NoDep,
    DK_Last
};

enum class LatencyTable : std::uint8_t
{
    V,
    Vpair,
    N,
    Npair,
    B,
    U,
    SMR,
    GCC,
    HW_REG,
    ACR,
    Internal_ACR,
    Vquad,
    RV,
    Custom,
    Floating,
    Unknown
};

struct LatencyInfoKey
{
    std::uint32_t pc;
    std::uint8_t slot;

    friend bool operator<(const LatencyInfoKey &a, const LatencyInfoKey &b)
    {
        return a.pc < b.pc || (!(b.pc < a.pc) && a.slot < b.slot);
    }
};

struct LatencyInfoStruct
{
    LatencyTable table;
    std::uint8_t producerIdx;
    std::uint8_t consumerIdx;
    std::uint8_t latency;
};

struct LatencyInfoLoc
{
    LatencyInfoKey loc;
    LatencyInfoStruct info;

    friend bool operator<(const LatencyInfoLoc &a, const LatencyInfoLoc &b)
    {
        return a.loc < b.loc;
    }
};

const char * const LATENCY_INFO_SECTION_NAME = ".mvpu.dbg.latency";

struct LatencyInfo
{
    std::vector<LatencyInfoLoc> inner;
};

// src info
const char * const SRC_INFO_SECTION_NAME = ".mvpu.dbg.src";

using SrcList = std::vector<std::string>;
using SrcInfo = std::unordered_map<std::string, SrcList>;

// cfg info
struct BasicBlockRange
{
    std::uint32_t first;
    std::uint32_t last;

    MVPU_ELF_ENTRY
    std::size_t hash() const;

    friend bool operator==(const BasicBlockRange &a, const BasicBlockRange &b)
    {
        return    a.first == b.first
               && a.last == b.last;
    }

    friend bool operator<(const BasicBlockRange &a, const BasicBlockRange &b)
    {
        return a.first < b.first || (!(b.first < a.first) && a.last < b.last);
    }

    template <typename H>
    friend H AbslHashValue(H state, const BasicBlockRange& bb)
    {
        return H::combine(absl::move(state), bb.first, bb.last);
    }
};

const char * const CFG_INFO_SECTION_NAME = ".mvpu.dbg.cfg";

struct ControlFlowGraph
{
    std::vector<std::tuple<BasicBlockRange, BasicBlockRange>> inner;
};

// symbol address
struct SymbolAddr
{
    enum Space : std::uint8_t
    {
        CONST_MEM,
        LOCAL_MEM,
    };

    Space space;
    std::uint32_t addr;
};

const char * const SYM_INFO_SECTION_NAME = ".mvpu.dbg.sym";

using SymbolInfo = std::unordered_map<std::string, SymbolAddr>;

// comment
const char * const COMMENT_SECTION_NAME = ".comment";

using CommentInfo = std::vector<absl::string_view>;

// others
const char * const PM_SIZE_SECTION_NAME = ".mvpu.dbg.pmsize";
const char * const VERIFY_INFO_SECTION_NAME = ".mvpu.dbg.verifyinfo";
} // namespace mvpu_elf

namespace std
{

template <>
struct hash<mvpu_elf::DebugInfoID>
{
    using ValTy = mvpu_elf::DebugInfoID;

    std::size_t operator()(const ValTy &id) const
    {
        return id.hash();
    }
}; // struct hash

template <>
struct hash<mvpu_elf::BasicBlockRange>
{
    using ValTy = mvpu_elf::BasicBlockRange;

    std::size_t operator()(const ValTy &bb) const
    {
        return bb.hash();
    }
}; // struct hash

} // namespace std

#endif // MVPU_ELF_CUSTOM_INFO_H
