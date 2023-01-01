
#ifndef CL_COMPILER_RAWBUFFER_H
#define CL_COMPILER_RAWBUFFER_H

#include "PrimeLib/Memory.h"

#include <cstring>
#include <vector>

namespace cl_compiler
{

class RawBuffer
{
private:
    std::vector<char> Buf;
    std::vector<unsigned int> PtrOffsetList;
    unsigned int AlignInBytes;

    void resizeInBytes(unsigned int Bytes) { Buf.resize((Bytes + (AlignInBytes - 1)) & ~(AlignInBytes - 1), 0); }
    unsigned int sizeInBytes() { return Buf.size(); }

public:
    RawBuffer(unsigned int Bytes, unsigned int Align = 4) : AlignInBytes(Align) { resizeInBytes(Bytes); }
    void *getPtr() { return sizeInBytes() ? (void *)&Buf[0] : 0; }
    static void fixPtr(void *Ptr, unsigned int PtrFixCount, unsigned int PtrFixOffset)
    {
        // FIXME: alignment
        unsigned int *PtrFixList = (unsigned int *)((char *)Ptr + PtrFixOffset);
        for (unsigned int i = 0; i < PtrFixCount; i += 2)
        {
            unsigned int PtrOffset = PtrFixList[i];
            unsigned int Offset = PtrFixList[i + 1];
            // FIXME: alignment
            *(char **)&((char *)Ptr)[PtrOffset] = (char *)Ptr + Offset;
        }
    }
    void *flush(unsigned int PtrFixCntOffset, unsigned int PtrFixOftOffset, unsigned int SizeOffset)
    {
        unsigned int PtrFixOft = -1;
        if (PtrFixCntOffset != (unsigned) - 1 && PtrOffsetList.size() != 0)
        {
            void *Dummy;
            PtrFixOft = copy(&Dummy, (void *)&PtrOffsetList[0], PtrOffsetList.size() * sizeof(PtrOffsetList[0]));
        }
        unsigned int Size = sizeInBytes();
        void *Ptr = CREATE_MEM(char, Size);
        assert(Ptr);
        std::memcpy(Ptr, &Buf[0], Size);
        if (PtrFixOft != (unsigned) - 1)
            fixPtr(Ptr, PtrOffsetList.size(), PtrFixOft);
        if (PtrFixCntOffset != (unsigned) - 1)
        {
            // FIXME: alignment
            *(unsigned int *)&((char *)Ptr)[PtrFixCntOffset] = PtrOffsetList.size();
            *(unsigned int *)&((char *)Ptr)[PtrFixOftOffset] = PtrFixOft;
        }
        if (SizeOffset != (unsigned) - 1)
            *(unsigned int *)&((char *)Ptr)[SizeOffset] = Size;
        return Ptr;
    }
    unsigned int allocate(void **HeadPtr, unsigned int PtrOffset, unsigned int Bytes)
    {
        unsigned int Offset = sizeInBytes();
        resizeInBytes(Offset + Bytes);
        PtrOffsetList.push_back(PtrOffset);
        PtrOffsetList.push_back(Offset);
        *HeadPtr = getPtr();
        return Offset;
    }
    unsigned int copy(void **HeadPtr, void *Src, unsigned int Bytes)
    {
        unsigned int Offset = sizeInBytes();
        if (Bytes)
        {
            resizeInBytes(Offset + Bytes);
            std::memcpy(&Buf[Offset], Src, Bytes);
            *HeadPtr = getPtr();
        }
        return Offset;
    }
    unsigned int copy(void **HeadPtr, unsigned int PtrOffset, void *Src, unsigned int Bytes)
    {
        unsigned int Offset = copy(HeadPtr, Src, Bytes);
        PtrOffsetList.push_back(PtrOffset);
        PtrOffsetList.push_back(Offset);
        return Offset;
    }
    void copyStrInArrayElement(void **HeadPtr, unsigned int PtrOffset, unsigned int FieldOffset, unsigned int ElementSize, void *Src, unsigned int Count)
    {
        char *Ptr = (char *)Src;
        PtrOffset += FieldOffset;
        Ptr += FieldOffset;
        for (unsigned int i = 0; i < Count; i++, PtrOffset += ElementSize, Ptr += ElementSize)
        {
            char *Str = *(char **)Ptr;
            copy(HeadPtr, PtrOffset, (void *)Str, strlen(Str) + 1);
        }
    }
};

} // namespace cl_compiler

#endif // ! CL_COMPILER_RAWBUFFER_H
