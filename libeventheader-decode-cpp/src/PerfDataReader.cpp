// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <PerfDataDecode/PerfDataReader.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <stdlib.h>
#define bswap_16(n) _byteswap_ushort(n)
#define bswap_32(n) _byteswap_ulong(n)
#define bswap_64(n) _byteswap_uint64(n)
static bool constexpr HostIsBigEndian = false;
#else // _WIN32
#include <byteswap.h>
#include <endian.h>
static bool constexpr HostIsBigEndian = __BYTE_ORDER == __BIG_ENDIAN;
#endif // _WIN32

PerfDataReader::PerfDataReader() noexcept
    : m_bigEndian(HostIsBigEndian) {}

PerfDataReader::PerfDataReader(bool bigEndian) noexcept
    : m_bigEndian(bigEndian) {}

bool
PerfDataReader::BigEndian() const noexcept
{
    return m_bigEndian;
}

bool
PerfDataReader::ByteSwapNeeded() const noexcept
{
    return HostIsBigEndian != m_bigEndian;
}

uint8_t
PerfDataReader::ReadAsU8(_In_reads_bytes_(1) void const* pSrc) const noexcept
{
    return *static_cast<uint8_t const*>(pSrc);
}

uint16_t
PerfDataReader::ReadAsU16(_In_reads_bytes_(2) void const* pSrc) const noexcept
{
    uint16_t fileBits;
    memcpy(&fileBits, pSrc, sizeof(fileBits));
    return HostIsBigEndian == m_bigEndian ? fileBits : bswap_16(fileBits);
}

uint32_t
PerfDataReader::ReadAsU32(_In_reads_bytes_(4) void const* pSrc) const noexcept
{
    uint32_t fileBits;
    memcpy(&fileBits, pSrc, sizeof(fileBits));
    return HostIsBigEndian == m_bigEndian ? fileBits : bswap_32(fileBits);
}

uint64_t
PerfDataReader::ReadAsU64(_In_reads_bytes_(8) void const* pSrc) const noexcept
{
    uint64_t fileBits;
    memcpy(&fileBits, pSrc, sizeof(fileBits));
    return HostIsBigEndian == m_bigEndian ? fileBits : bswap_64(fileBits);
}

uint32_t
PerfDataReader::ReadAsDynU32(
    _In_reads_bytes_(cbSrc) void const* pSrc,
    uint8_t cbSrc) const noexcept
{
    uint32_t result;
    switch (cbSrc)
    {
    default:
        assert(false);
        result = 0;
        break;
    case 1:
        result = *static_cast<uint8_t const*>(pSrc);
        break;
    case 2:
        result = ReadAsU16(pSrc);
        break;
    case 4:
        result = ReadAsU32(pSrc);
        break;
    }
    return result;
}

uint64_t
PerfDataReader::ReadAsDynU64(
    _In_reads_bytes_(cbSrc) void const* pSrc,
    uint8_t cbSrc) const noexcept
{
    uint64_t result;
    switch (cbSrc)
    {
    default:
        assert(false);
        result = 0;
        break;
    case 1:
        result = *static_cast<uint8_t const*>(pSrc);
        break;
    case 2:
        result = ReadAsU16(pSrc);
        break;
    case 4:
        result = ReadAsU32(pSrc);
        break;
    case 8:
        result = ReadAsU64(pSrc);
        break;
    }
    return result;
}
