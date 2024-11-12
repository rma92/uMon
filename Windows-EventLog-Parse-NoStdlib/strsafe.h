#ifdef __cplusplus
#define _STRSAFE_EXTERN_C extern "C"
#else
#define _STRSAFE_EXTERN_C
#endif

#ifndef WINAPI
#if defined(_ARM_)
#define WINAPI
#else
#define WINAPI __stdcall
#endif
#endif

typedef char *STRSAFE_LPSTR;
typedef const char *STRSAFE_LPCSTR;
typedef wchar_t *STRSAFE_LPWSTR;
typedef const wchar_t *STRSAFE_LPCWSTR; 

#define STRSAFEAPI _STRSAFE_EXTERN_C __inline HRESULT WINAPI

STRSAFEAPI StringCchCopyA(STRSAFE_LPSTR pszDest,size_t cchDest,STRSAFE_LPCSTR pszSrc);
STRSAFEAPI StringCchCopyW(STRSAFE_LPWSTR pszDest,size_t cchDest,STRSAFE_LPCWSTR pszSrc);

