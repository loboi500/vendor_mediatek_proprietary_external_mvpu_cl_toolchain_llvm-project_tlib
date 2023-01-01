#ifndef CL_COMPILER_UTILITIES_H
#define CL_COMPILER_UTILITIES_H

#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/raw_ostream.h"

#include "PrimeLib/CString.h"
#include "PrimeLib/string_view.h"
#include "PrimeLib/type_traits.h"

#include "absl/hash/hash.h"
#include "absl/types/span.h"

#include "fmt/format.h"

namespace cl_compiler
{

inline llvm::StringRef toStringRef(absl::string_view s)
{
    return llvm::StringRef(s.data(), s.size());
}

inline llvm::StringRef toStringRef(const prime_lib::CString<> &s)
{
    return llvm::StringRef(s.data(), s.size());
}

inline absl::string_view toStringView(llvm::StringRef s)
{
    return absl::string_view(s.data(), s.size());
}

namespace detail
{

template <typename T>
inline llvm::ArrayRef<T> toArrayRef(const T *p, size_t s)
{
    return llvm::makeArrayRef(p, s);
}

template <typename T>
inline llvm::ArrayRef<T> toArrayRef(T *p, size_t s)
{
    return llvm::makeMutableArrayRef(p, s);
}

} // namespace detail

template <typename T>
inline auto toArrayRef(absl::Span<T> a)
    -> decltype(detail::toArrayRef(a.data(), a.size()))
{
    return detail::toArrayRef(a.data(), a.size());
}

} // namespace cl_compiler

namespace fmt
{

template <>
struct formatter<llvm::StringRef> : public formatter<basic_string_view<char>, char>
{
    template <typename FormatContext>
    auto format(llvm::StringRef s, FormatContext &ctx) -> decltype(ctx.out())
    {
        basic_string_view<char> str(s.data(), s.size());
        return formatter<basic_string_view<char>, char>::format(str, ctx);
    }
};

} // namespace fmt

namespace llvm
{

template <typename H>
inline H AbslHashValue(H state, StringRef str)
{
    return H::combine(absl::move(state), cl_compiler::toStringView(str));
}

inline raw_ostream &operator<<(raw_ostream &os, const prime_lib::CString<> &s)
{
    return os << cl_compiler::toStringRef(s);
}

inline raw_ostream &operator<<(raw_ostream &os, absl::string_view s)
{
    return os << cl_compiler::toStringRef(s);
}

} // namespace llvm

#endif // CL_COMPILER_UTILITIES_H
