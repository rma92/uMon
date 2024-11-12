#ifndef STRING_UTILS_H
#define STRING_UTILS_H


#define STRSAFE_E_INSUFFICIENT_BUFFER ((HRESULT)0x8007007A)
#define STRSAFE_E_INVALID_PARAMETER ((HRESULT)0x80070057)
#define STRSAFE_E_END_OF_FILE ((HRESULT)0x80070026)

#include <stddef.h>
#include <stdarg.h>


// Check if we're on Windows and if Kernel32 functions are available
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>

    // Use Windows API functions if available
    static inline size_t Strlen(const char* str) {
        return (size_t)lstrlenA(str);
    }

    static inline size_t Wcslen(const wchar_t* str) {
        return (size_t)lstrlenW(str);
    }

    // Use Windows API function RtlCopyMemory if available
    static inline void* Memcpy(void* dest, const void* src, size_t count) {
        RtlCopyMemory(dest, src, count);
        return dest;
    }
//End if Win32
#else
    // Fallback to custom implementations if Windows API is unavailable
    static inline size_t Strlen(const char* str) {
        size_t length = 0;
        while (str[length] != '\0') {
            length++;
        }
        return length;
    }

    static inline size_t Wcslen(const wchar_t* str) {
        size_t length = 0;
        while (str[length] != L'\0') {
            length++;
        }
        return length;
    }

    // Fallback to custom memcpy implementation if Windows API is unavailable
    static inline void* Memcpy(void* dest, const void* src, size_t count) {
        unsigned char* d = (unsigned char*)dest;
        const unsigned char* s = (const unsigned char*)src;
        while (count--) {
            *d++ = *s++;
        }
        return dest;
    }

#endif // Windows API check

#endif // STRING_UTILS_H

HRESULT SafeStringCopyA(char* pszDest, size_t cchDest, const char* pszSrc) {
    if (pszDest == NULL || pszSrc == NULL) {
        return E_INVALIDARG;  // Invalid arguments
    }
    
    size_t srcLen = Strlen(pszSrc);
    if (srcLen + 1 > cchDest) {
        if (cchDest > 0) {
            pszDest[0] = '\0';  // Null terminate the destination buffer if possible
        }
        return STRSAFE_E_INSUFFICIENT_BUFFER;  // Destination buffer too small
    }

    Memcpy(pszDest, pszSrc, srcLen + 1);  // Copy string including null terminator
    return S_OK;
}

HRESULT SafeStringCopyW(wchar_t* pszDest, size_t cchDest, const wchar_t* pszSrc) {
    if (pszDest == NULL || pszSrc == NULL) {
        return E_INVALIDARG;  // Invalid arguments
    }
    
    size_t srcLen = Wcslen(pszSrc);
    if (srcLen + 1 > cchDest) {
        if (cchDest > 0) {
            pszDest[0] = L'\0';  // Null terminate the destination buffer if possible
        }
        return STRSAFE_E_INSUFFICIENT_BUFFER;  // Destination buffer too small
    }

    Memcpy(pszDest, pszSrc, (srcLen + 1) * sizeof(wchar_t));  // Copy string including null terminator
    return S_OK;
}

int win_printf(const char* format, ...) {
    char buffer[1024];  // Adjust size as needed
    va_list args;
    va_start(args, format);

    // Use wvsprintf to format the string with the provided arguments
    int len = wvsprintfA(buffer, format, args);
    va_end(args);

    // If formatting fails, return an error
    if (len < 0) {
        return len;
    }

    // Get the standard output handle
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        return -1;  // Error getting handle
    }

    DWORD written;
    // Check if the handle is a console or redirected
    DWORD mode;
    if (GetConsoleMode(hStdOut, &mode)) {
        // Output to console
        WriteConsoleA(hStdOut, buffer, len, &written, NULL);
    } else {
        // Output to a redirected handle (file, pipe, etc.)
        WriteFile(hStdOut, buffer, len, &written, NULL);
    }

    return written;
}
