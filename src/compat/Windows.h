#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#define __stdcall
#define CALLBACK
#define WINAPI
#define MAX_PATH 260
#define TRUE 1
#define FALSE 0
#define INVALID_HANDLE_VALUE ((HANDLE)(intptr_t)-1)
#define FAILED(hr) ((HRESULT)(hr) < 0)
#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)
#define TEXT(value) value
#define ZeroMemory(ptr, size) std::memset((ptr), 0, (size))
#define FillMemory(ptr, size, value) std::memset((ptr), (value), (size))
#define C_ASSERT(expr) static_assert(expr)
#define __int64 long long

typedef int BOOL;
typedef std::uint8_t BYTE;
typedef std::uint16_t WORD;
typedef std::uint32_t DWORD;
typedef std::uint32_t UINT;
typedef std::uint32_t ULONG;
typedef std::int32_t LONG;
typedef std::int32_t HRESULT;
typedef std::uintptr_t DWORD_PTR;
typedef std::uintptr_t WPARAM;
typedef std::intptr_t LPARAM;
typedef std::intptr_t LRESULT;
typedef void *HANDLE;
typedef void *HWND;
typedef void *HINSTANCE;
typedef void *HKEY;
typedef void *LPVOID;
typedef DWORD *LPDWORD;
typedef char *LPSTR;
typedef char *LPTSTR;
typedef const char *LPCSTR;
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
typedef char CHAR;

typedef struct _FILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;
typedef FILETIME *LPFILETIME;

typedef UINT MMRESULT;
typedef void *HMMIO;
typedef void *HMIDIOUT;

typedef struct timecaps_tag
{
    UINT wPeriodMin;
    UINT wPeriodMax;
} TIMECAPS;

typedef void(CALLBACK *LPTIMECALLBACK)(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);

typedef struct midihdr_tag
{
    char *lpData;
    DWORD dwBufferLength;
    DWORD dwBytesRecorded;
    DWORD_PTR dwUser;
    DWORD dwFlags;
    struct midihdr_tag *lpNext;
    DWORD_PTR reserved;
    DWORD dwOffset;
    DWORD_PTR dwReserved[8];
} MIDIHDR;

typedef MIDIHDR *LPMIDIHDR;

inline BOOL GetFileTime(HANDLE, LPFILETIME, LPFILETIME, LPFILETIME)
{
    return FALSE;
}

typedef struct _GUID
{
    DWORD Data1;
    WORD Data2;
    WORD Data3;
    BYTE Data4[8];
} GUID;

typedef struct tagRECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT;

typedef struct tagPOINT
{
    LONG x;
    LONG y;
} POINT;
