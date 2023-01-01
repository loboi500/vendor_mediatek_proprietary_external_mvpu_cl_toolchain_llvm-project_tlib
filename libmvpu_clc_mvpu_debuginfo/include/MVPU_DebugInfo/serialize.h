// SPDX-License-Identifier: Apache-2.0
#ifndef MVPU_DEBUG_INFO_SERIALIZE_H
#define MVPU_DEBUG_INFO_SERIALIZE_H

#include "MVPU_DebugInfo/DebugInfoList.h"

#include "MVPU_ELF/serialize.h"

#include "PrimeLib/Serialize.h"
#include "PrimeLib/Try.h"

#include "absl/base/macros.h"
#include "absl/meta/type_traits.h"

namespace prime_lib { namespace serialize {

#define ENCODE_V2(t, v, e) Encodable<t>::encode(v, e)
#define ENCODE_V(v, e) ENCODE_V2(absl::decay_t<decltype(v)>, v, e)

#define DECODE_T(t, d) Decodable<t>::decode(d)
#define DECODE_V(v, d) DECODE_T(absl::decay_t<decltype(v)>, d)

// RelocELF
template <>
struct Encodable<::mvpu_debuginfo::RelocELF>
{
    using ValTy = ::mvpu_debuginfo::RelocELF;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(ValTy &reloc, E &e)
    {
        PRIME_LIB_TRYV(ENCODE_V(reloc.elf, e));
        PRIME_LIB_TRYV(ENCODE_V(reloc.secAddrs, e));

        return RetTy::Ok(Unit{});
    }
};

template <>
struct Decodable<::mvpu_debuginfo::RelocELF>
{
    using ValTy = ::mvpu_debuginfo::RelocELF;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ABSL_INTERNAL_TRY
        {
            ValTy reloc;

            PRIME_LIB_TRY(elf     , DECODE_V(reloc.elf, d));
            PRIME_LIB_TRY(secAddrs, DECODE_V(reloc.secAddrs, d));

            reloc.elf = std::move(elf);
            reloc.secAddrs = std::move(secAddrs);

            return RetTy::Ok(std::move(reloc));
        }
        ABSL_INTERNAL_CATCH_ANY
        {
            using ErrTy = typename D::ErrTy;
            return RetTy::Err(ErrTy{});
        }
    }
};

// DebugInfoList::IDELFMap
template <>
struct Encodable<::mvpu_debuginfo::DebugInfoList::IDELFMap>
{
    using ValTy = ::mvpu_debuginfo::DebugInfoList::IDELFMap;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(ValTy &map, E &e)
    {
        using Key = typename ValTy::key_type;
        using Val = typename ValTy::mapped_type;

        return e.emitMap(map.size(), [&map](E &mapE)
        {
            std::size_t ix = 0;
            for (auto &pair : map)
            {
                {
                    auto res = mapE.emitMapKey(ix, [&pair](E &keyE) { return Encodable<Key>::encode(pair.first, keyE); });
                    if (res.isErr())
                        return RetTy::Err(std::move(res.getErr()));
                }
                {
                    auto res = mapE.emitMapVal(ix, [&pair](E &valE) { return Encodable<Val>::encode(pair.second, valE); });
                    if (res.isErr())
                        return RetTy::Err(std::move(res.getErr()));
                }
                ix += 1;
            }
            return RetTy::Ok(Unit{});
        });
    }
};

template <>
struct Decodable<::mvpu_debuginfo::DebugInfoList::IDELFMap>
{
    using ValTy = ::mvpu_debuginfo::DebugInfoList::IDELFMap;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        using Key = typename ValTy::key_type;
        using Val = typename ValTy::mapped_type;

        return d.template readMap<ValTy>([](D &mapD, std::size_t len)
        {
            ValTy map;
            for (unsigned i = 0; i < len; ++i)
            {
                auto key = mapD.template readMapKey<Key>(i, [](D &keyD) { return Decodable<Key>::decode(keyD); });
                if (key.isErr())
                    return RetTy::Err(std::move(key.getErr()));
                auto val = mapD.template readMapVal<Val>(i, [](D &valD) { return Decodable<Val>::decode(valD); });
                if (val.isErr())
                    return RetTy::Err(std::move(val.getErr()));
                map.emplace(std::move(key.getOk()), std::move(val.getOk()));
            }
            return RetTy::Ok(std::move(map));
        });
    }
};

// DebugInfoList
template <>
struct Encodable<::mvpu_debuginfo::DebugInfoList>
{
    using ValTy = ::mvpu_debuginfo::DebugInfoList;
    template<typename E, typename RetTy = Result<Unit, typename E::ErrTy>>
    static RetTy encode(ValTy &list, E &e)
    {
        PRIME_LIB_TRYV(ENCODE_V(list.infoMap, e));

        return RetTy::Ok(Unit{});
    }
};

template<>
struct Decodable<::mvpu_debuginfo::DebugInfoList>
{
    using ValTy = ::mvpu_debuginfo::DebugInfoList;
    template<typename D, typename RetTy = Result<ValTy, typename D::ErrTy>>
    static RetTy decode(D &d)
    {
        ValTy list;

        PRIME_LIB_TRY(infoMap, DECODE_V(list.infoMap, d));

        list.infoMap = std::move(infoMap);

        return RetTy::Ok(std::move(list));
    }
};

#undef DECODE_V
#undef DECODE_T

#undef ENCODE_V
#undef ENCODE_V2

}} // namespace prime_lib::serialize

#endif // MTKVPU_DEBUG_INFO_SERIALIZE_H
