/*++
Copyright (c) Microsoft Corporation

Module Name:
- AttrRow.hpp

Abstract:
- contains data structure for the attributes of one row of screen buffer

Author(s):
- Michael Niksa (miniksa) 10-Apr-2014
- Paul Campbell (paulcam) 10-Apr-2014

Revision History:
- From components of output.h/.c
  by Therese Stowell (ThereseS) 1990-1991
- Pulled into its own file from textBuffer.hpp/cpp (AustDi, 2017)
--*/

#pragma once

#include "TextAttributeRun.hpp"
#include <vector>

class ATTR_ROW final
{
public:
    ATTR_ROW(const UINT cchRowWidth, const TextAttribute attr);
    ATTR_ROW(const ATTR_ROW& a);
    ATTR_ROW& operator=(const ATTR_ROW& a);
    ATTR_ROW(ATTR_ROW&& a) noexcept = default;

    void swap(ATTR_ROW& other) noexcept;

    bool Reset(const TextAttribute attr);

    TextAttribute at(const size_t column) const;
    void FindAttrIndex(const size_t index,
                       _Outptr_ TextAttributeRun** const ppIndexedAttr,
                       _Out_opt_ size_t* const pcAttrApplies) const;
    bool SetAttrToEnd(const UINT iStart, const TextAttribute attr);
    void ReplaceLegacyAttrs(const WORD wToBeReplacedAttr, const WORD wReplaceWith);
    [[nodiscard]]
    HRESULT Resize(const short sOldWidth, const short sNewWidth);

    [[nodiscard]]
    HRESULT InsertAttrRuns(_In_reads_(cAttrs) const TextAttributeRun* const prgAttrs,
                           const size_t cAttrs,
                           const size_t iStart,
                           const size_t iEnd,
                           const size_t cBufferWidth);

    [[nodiscard]]
    NTSTATUS UnpackAttrs(_Out_writes_(cRowLength) TextAttribute* const rgAttrs, const size_t cRowLength) const;

    static std::vector<TextAttributeRun> PackAttrs(const std::vector<TextAttribute>& attrs);

    friend constexpr bool operator==(const ATTR_ROW& a, const ATTR_ROW& b) noexcept;

    size_t _cList;   // length of attr pair array
    wistd::unique_ptr<TextAttributeRun[]> _rgList;
private:

    size_t _cchRowWidth;


#ifdef UNIT_TESTING
    friend class AttrRowTests;
#endif

};

void swap(ATTR_ROW& a, ATTR_ROW& b) noexcept;
constexpr bool operator==(const ATTR_ROW& a, const ATTR_ROW& b) noexcept
{
    return (a._cList == b._cList &&
            a._rgList == b._rgList &&
            a._cchRowWidth == b._cchRowWidth);
}

// this sticks specialization of swap() into the std::namespace for ATTR_ROW, so that callers that use
// std::swap explicitly over calling the global swap can still get the performance benefit.
namespace std
{
    template<>
    inline void swap<ATTR_ROW>(ATTR_ROW& a, ATTR_ROW& b) noexcept
    {
        a.swap(b);
    }
}
