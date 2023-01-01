// SPDX-License-Identifier: Apache-2.0
#ifndef VPU_ISA_DISASSEMBLER_H
#define VPU_ISA_DISASSEMBLER_H

#include "VPU_ISA/Common.h"

#include "ISA_API.h"

#include "PrimeLib/Option.h"
#include "PrimeLib/string_view.h"

#include "absl/types/span.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace vpu_isa
{

struct DebugLineInfo
{
    std::string file;
    std::uint8_t opIndex;
    std::uint32_t line;
    std::uint32_t column;
};

class ISA_API_ENTRY DecodedInst
{
public:
    DecodedInst(const DecodedInst &) = delete;

    DecodedInst(DecodedInst &&);

    ~DecodedInst();

    DecodedInst & operator=(const DecodedInst &) = delete;

    DecodedInst & operator=(DecodedInst &&);

    BytesRef getData() const { return data; }

    std::uint64_t getAddr() const { return addr; }

    std::uint64_t getBrTarget() const { return brTarget; }

    bool isUnknown() const;

    std::string toString() const;

private:
    friend class DecodedSymbol;
    friend class Disassembler;

    struct Impl;

    DecodedInst(BytesRef data, std::uint64_t addr, std::uint64_t brTarget);

    std::unique_ptr<Impl> impl;
    std::vector<unsigned char> data;
    std::uint64_t addr = 0;
    std::uint64_t brTarget = 0;
}; // class DecodedInst

class ISA_API_ENTRY DecodedSymbol
{
public:
    using InstList = std::vector<DecodedInst>;

    enum class Type : std::uint8_t
    {
        NoType,
        Func,
    };

    DecodedSymbol(const DecodedSymbol &) = delete;

    DecodedSymbol(DecodedSymbol &&);

    ~DecodedSymbol();

    DecodedSymbol & operator=(const DecodedSymbol &) = delete;

    DecodedSymbol & operator=(DecodedSymbol &&);

    absl::string_view getName() const { return name; }

    std::uint64_t getAddr() const { return addr; }

    std::uint64_t getSize() const { return size; }

    Type getType() const { return type; }

    InstList & getInsts() { return insts; }

    const InstList & getInsts() const { return insts; }

private:
    friend class DecodedObject;
    friend class Disassembler;

    struct Impl;

    DecodedSymbol(absl::string_view name, std::uint64_t addr, std::uint64_t size, Type ty);

    DecodedInst makeInst(BytesRef data, std::uint64_t addr);

    void addInst(DecodedInst inst);

    std::unique_ptr<Impl> impl;
    std::string name;
    std::uint64_t addr = 0;
    std::uint64_t size = 0;
    Type type;
    InstList insts;
}; // class DecodedSymbol

class ISA_API_ENTRY DecodedObject
{
public:
    using SymbolList = std::vector<DecodedSymbol>;

    DecodedObject(const DecodedObject &) = delete;

    DecodedObject(DecodedObject &&);

    ~DecodedObject();

    DecodedObject & operator=(const DecodedObject &) = delete;

    DecodedObject & operator=(DecodedObject &&);

    SymbolList & getSymbols() { return symbols; }

    const SymbolList & getSymbols() const { return symbols; }

    std::vector<DebugLineInfo> getLineInfo(std::uint64_t addr, std::uint64_t size);

private:
    friend class Disassembler;

    struct Impl;

    DecodedObject(std::unique_ptr<Impl> impl);

    DecodedSymbol makeSymbol(absl::string_view name, std::uint64_t addr,
                             std::uint64_t size, DecodedSymbol::Type ty);

    void addSymbol(DecodedSymbol sym);

    std::unique_ptr<Impl> impl;
    SymbolList symbols;
}; // class DecodedObject

class ISA_API_ENTRY Disassembler
{
public:
    static prime_lib::Option<Disassembler> make();

    Disassembler(const Disassembler &) = delete;

    Disassembler(Disassembler &&);

    ~Disassembler();

    Disassembler & operator=(const Disassembler &) = delete;

    Disassembler & operator=(Disassembler &&);

    prime_lib::Option<DecodedObject> decode(BytesRef bin) const;

private:
    struct Impl;

    Disassembler();

    std::unique_ptr<Impl> impl;
}; // class Disassembler

} // namespace vpu_isa

#endif // ! VPU_ISA_DISASSEMBLER_H
