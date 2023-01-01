#ifndef MVPU_DEBUG_INFO_DEBUG_INFO_LIST_H
#define MVPU_DEBUG_INFO_DEBUG_INFO_LIST_H

#include "MVPU_DebugInfo.h"

#include "MVPU_ELF/ELF.h"

#include "PrimeLib/Option.h"
#include "PrimeLib/Serialize.h"

#include "absl/types/span.h"

#include <unordered_map>

namespace mvpu_debuginfo
{

using ::mvpu_elf::DebugInfoID;
using ::mvpu_elf::ELF;

struct DebugInfoNewID
{
    DebugInfoID old;
    DebugInfoID new_;
};

struct RelocELF
{
    using SectionAddrMap = std::unordered_map<std::string, std::uint32_t>;

    MVPU_DBG_ENTRY
    RelocELF() = default;

    RelocELF(const RelocELF &) = delete;

    MVPU_DBG_ENTRY
    RelocELF(RelocELF &&) = default;

    MVPU_DBG_ENTRY
    RelocELF(ELF &&elf) : elf(std::move(elf)) {}

    MVPU_DBG_ENTRY
    ~RelocELF() = default;

    RelocELF & operator=(const RelocELF &) = delete;

    MVPU_DBG_ENTRY
    RelocELF & operator=(RelocELF &&) = default;

    ELF elf;
    SectionAddrMap secAddrs;
};

class DebugInfoList
{
private:
    using IDELFMap = std::unordered_map<DebugInfoID, RelocELF>;

public:
    using key_type = IDELFMap::key_type;
    using mapped_type = IDELFMap::mapped_type;
    using value_type = IDELFMap::value_type;
    using size_type = IDELFMap::size_type;
    using difference_type = IDELFMap::difference_type;
    using iterator = IDELFMap::iterator;

    struct Range
    {
        iterator begin;
        iterator end;
    };

    MVPU_DBG_ENTRY
    DebugInfoList() = default;

    DebugInfoList(const DebugInfoList &) = delete;

    MVPU_DBG_ENTRY
    DebugInfoList(DebugInfoList &&) = default;

    MVPU_DBG_ENTRY
    ~DebugInfoList() = default;

    DebugInfoList & operator=(const DebugInfoList &) = delete;

    MVPU_DBG_ENTRY
    DebugInfoList & operator=(DebugInfoList &&) = default;

    MVPU_DBG_ENTRY
    iterator begin() { return infoMap.begin(); }

    MVPU_DBG_ENTRY
    iterator end() { return infoMap.end(); }

    MVPU_DBG_ENTRY
    size_type size() const { return infoMap.size(); }

    MVPU_DBG_ENTRY
    bool empty() const { return infoMap.empty(); }

    MVPU_DBG_ENTRY
    ELF * add(const DebugInfoID &id, ELF &&elf);

    MVPU_DBG_ENTRY
    bool merge(DebugInfoList &other);

    MVPU_DBG_ENTRY
    bool update(absl::Span<const DebugInfoNewID> idList);

    MVPU_DBG_ENTRY
    ELF * find(const DebugInfoID &id);

    MVPU_DBG_ENTRY
    void clear();

    MVPU_DBG_ENTRY
    std::uint32_t getSectionAddr(const DebugInfoID &id, const std::string &sec) const;

    MVPU_DBG_ENTRY
    void setSectionAddr(const DebugInfoID &id, const std::string &sec, std::uint32_t addr);

    friend ::prime_lib::serialize::Encodable<IDELFMap>;
    friend ::prime_lib::serialize::Decodable<IDELFMap>;

    friend ::prime_lib::serialize::Encodable<DebugInfoList>;
    friend ::prime_lib::serialize::Decodable<DebugInfoList>;

private:
    IDELFMap infoMap;
}; // DebugInfoList

} // namespace mvpu_debuginfo

#endif // MVPU_DEBUG_INFO_DEBUG_INFO_LIST_H
