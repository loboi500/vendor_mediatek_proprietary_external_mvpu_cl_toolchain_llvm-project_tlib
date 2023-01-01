// SPDX-License-Identifier: Apache-2.0
#ifndef MVPU_ELF_ELF_H
#define MVPU_ELF_ELF_H

#include "MVPU_ELF.h"
#include "MVPU_ELF/CustomInfo.h"

#include "PrimeLib/Option.h"
#include "PrimeLib/FixedVector.h"

#include "absl/types/span.h"

#include <cstdint>
#include <memory>
#include <string>

namespace mvpu_elf
{

enum class Machine : std::uint16_t
{
    MTKRV5 = 0xf3,
    MTKVPU = 0x2454
};

enum class Type : std::uint16_t
{
    REL  = 1,
    EXEC = 2,
    DYN  = 3,
};

class ELF
{
public:
    using RawData = absl::Span<const unsigned char>;

private:
    using DebugInfoIDOpt = prime_lib::Option<DebugInfoID>;
    using TextOpt = prime_lib::Option<RawData>;
    using SpillInfoOpt = prime_lib::Option<SpillInfo>;
    using SWPInfoOpt = prime_lib::Option<SWPInfo>;
    using CFGInfoOpt = prime_lib::Option<ControlFlowGraph>;
    using SrcInfoOpt = prime_lib::Option<SrcInfo>;
    using SymbolInfoOpt = prime_lib::Option<SymbolInfo>;
    using PMSizeOpt = prime_lib::Option<unsigned int>;
    using VerifyInfoOpt = prime_lib::Option<std::string>;
    using LatencyInfoOpt = prime_lib::Option<LatencyInfo>;
    using CommentInfoOpt = prime_lib::Option<CommentInfo>;

public:
    using Buffer = prime_lib::FixedVector<unsigned char>;
    using BufferOpt = prime_lib::Option<Buffer>;
    using ELFOpt = prime_lib::Option<ELF>;

    MVPU_ELF_ENTRY
    ELF();

    ELF(const ELF &) = delete;

    MVPU_ELF_ENTRY
    ELF(ELF &&);

    MVPU_ELF_ENTRY
    ~ELF();

    ELF & operator=(const ELF &) = delete;

    MVPU_ELF_ENTRY
    ELF & operator=(ELF &&);

    // elf binary
    MVPU_ELF_ENTRY
    static ELFOpt load(RawData buf);

    MVPU_ELF_ENTRY
    BufferOpt save();

    // machine
    MVPU_ELF_ENTRY
    std::uint16_t getMachine() const;

    MVPU_ELF_ENTRY
    void setMachine(std::uint16_t em);

    // type
    MVPU_ELF_ENTRY
    std::uint16_t getType() const;

    MVPU_ELF_ENTRY
    void setType(std::uint16_t type);

    // entry
    MVPU_ELF_ENTRY
    std::uint64_t getEntry() const;

    MVPU_ELF_ENTRY
    void setEntry(std::uint64_t entry);

    // debug info id section
    MVPU_ELF_ENTRY
    bool hasDebugInfoID() const { return id.hasVal(); };

    MVPU_ELF_ENTRY
    const DebugInfoID & getDebugInfoID() const { return id.getVal(); };

    MVPU_ELF_ENTRY
    void setDebugInfoID(const DebugInfoID &id) { this->id = DebugInfoIDOpt::Some(id); }

    // spill info section
    MVPU_ELF_ENTRY
    bool hasSpillInfo() const { return spill.hasVal(); };

    MVPU_ELF_ENTRY
    const SpillInfo & getSpillInfo() const { return spill.getVal(); };

    MVPU_ELF_ENTRY
    void setSpillInfo(SpillInfo info) { this->spill = SpillInfoOpt::Some(std::move(info));  }

    // SWP info section
    MVPU_ELF_ENTRY
    bool hasSWPInfo() const { return swp.hasVal(); };

    MVPU_ELF_ENTRY
    const SWPInfo & getSWPInfo() const { return swp.getVal(); };

    MVPU_ELF_ENTRY
    void setSWPInfo(SWPInfo info) { this->swp = SWPInfoOpt::Some(std::move(info));  }

    // cfg section
    MVPU_ELF_ENTRY
    bool hasCFGInfo() const { return cfg.hasVal(); };

    MVPU_ELF_ENTRY
    const ControlFlowGraph & getCFGInfo() const { return cfg.getVal(); };

    MVPU_ELF_ENTRY
    void setCFGInfo(ControlFlowGraph cfg) { this->cfg = CFGInfoOpt::Some(std::move(cfg));  }

    // src info section
    MVPU_ELF_ENTRY
    bool hasSrcInfo() const { return src.hasVal(); };

    MVPU_ELF_ENTRY
    const SrcInfo & getSrcInfo() const { return src.getVal(); };

    MVPU_ELF_ENTRY
    void setSrcInfo(SrcInfo info) { this->src = SrcInfoOpt::Some(std::move(info));  }

    // symbol info section
    MVPU_ELF_ENTRY
    bool hasSymbolInfo() const { return sym.hasVal(); };

    MVPU_ELF_ENTRY
    const SymbolInfo & getSymbolInfo() const { return sym.getVal(); };

    MVPU_ELF_ENTRY
    void setSymbolInfo(SymbolInfo info) { this->sym = SymbolInfoOpt::Some(std::move(info));  }

    // private memory size section
    MVPU_ELF_ENTRY
    bool hasPMSizeInfo() const { return pmSize.hasVal(); };

    MVPU_ELF_ENTRY
    unsigned int getPMSizeInfo() const { return pmSize.getVal(); };

    MVPU_ELF_ENTRY
    void setPMSizeInfo(unsigned int size) { this->pmSize = PMSizeOpt::Some(std::move(size)); };

    // called functions info
    MVPU_ELF_ENTRY
    bool hasVerifyInfo() const { return verifyinfo.hasVal(); };

    MVPU_ELF_ENTRY
    const std::string & getVerifyInfo() const { return verifyinfo.getVal(); };

    MVPU_ELF_ENTRY
    void setVerifyInfo(std::string names) { this->verifyinfo = VerifyInfoOpt::Some(std::move(names)); };

    // latency info section
    MVPU_ELF_ENTRY
    bool hasLatencyInfo() const { return latency.hasVal(); };

    MVPU_ELF_ENTRY
    const LatencyInfo & getLatencyInfo() const { return latency.getVal(); };

    MVPU_ELF_ENTRY
    void setLatencyInfo(LatencyInfo latency) { this->latency = LatencyInfoOpt::Some(std::move(latency)); };

    // comment section
    MVPU_ELF_ENTRY
    bool hasCommentInfo() const { return comment.hasVal(); };

    MVPU_ELF_ENTRY
    const CommentInfo & getCommentInfo() const { return comment.getVal(); };

    MVPU_ELF_ENTRY
    void setCommentInfo(const CommentInfo &comment);

    // text section
    MVPU_ELF_ENTRY
    bool hasText() const { return text.hasVal(); };

    MVPU_ELF_ENTRY
    RawData getText() const { return text.getVal(); };

    MVPU_ELF_ENTRY
    std::uint32_t getTextAddr() const;

    MVPU_ELF_ENTRY
    void setText(RawData data, unsigned int alignment);

    // raw data section
    MVPU_ELF_ENTRY
    bool hasSection(const std::string &name) const;

    MVPU_ELF_ENTRY
    RawData getSection(const std::string &name) const;

    MVPU_ELF_ENTRY
    size_t getSectionSize(const std::string &name) const;

    MVPU_ELF_ENTRY
    void setSection(const std::string &name, RawData data);

    MVPU_ELF_ENTRY
    std::vector<std::string> getSectionNames() const;

private:
    struct Impl;

    std::unique_ptr<Impl> impl;

    DebugInfoIDOpt id;
    SpillInfoOpt spill;
    SWPInfoOpt swp;
    CFGInfoOpt cfg;
    SrcInfoOpt src;
    SymbolInfoOpt sym;
    PMSizeOpt pmSize;
    VerifyInfoOpt verifyinfo;
    LatencyInfoOpt latency;
    CommentInfoOpt comment;
    TextOpt text;
}; // class ELF

} // namespace mvpu_elf

#endif // MVPU_ELF_ELF_H
