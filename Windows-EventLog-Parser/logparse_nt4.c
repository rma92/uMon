#include <windows.h>
#include <stdio.h>
#include <time.h>

#ifndef EVENTLOG_SEQUENTIAL_READ
#define EVENTLOG_SEQUENTIAL_READ 0x0001
#endif

#ifndef EVENTLOG_SEEK_READ
#define EVENTLOG_SEEK_READ 0x0002
#endif

#ifndef EVENTLOG_FORWARDS_READ
#define EVENTLOG_FORWARDS_READ 0x0004
#endif

#ifndef LOAD_LIBRARY_AS_IMAGE_RESOURCE
#define LOAD_LIBRARY_AS_IMAGE_RESOURCE 0x0020
#endif

#ifndef STRSAFE_E_INSUFFICIENT_BUFFER
#define STRSAFE_E_INSUFFICIENT_BUFFER ((HRESULT)0x8007007A)
#endif

//open and format Windows Event Log
// tcc logparse.c -ladvapi32
//use the full windows api if there are compiling errors, but it should not use anything too new

HRESULT SafeStringCopy(char* dest, size_t destSize, const char* src) {
    size_t srcLen = strlen(src);
    if (srcLen + 1 > destSize) {
        return STRSAFE_E_INSUFFICIENT_BUFFER;  // Or another error code
    }
    memcpy(dest, src, srcLen + 1);  // Include the null terminator
    return S_OK;
}

LPWSTR GetFormattedMessage( DWORD dwMessageId, LPCSTR lpSourceName, LPVOID *lpStrings, DWORD dwNumStrings, LPCWSTR pszBuffer, int iBufferLen )
{
  LPWSTR pBuffer = NULL;
  HKEY hKey;
  CHAR szMessageFile[MAX_PATH];
  CHAR szExpandedMessageFile[MAX_PATH];
  DWORD dwBufferSize = sizeof( szMessageFile );

  CHAR szRegPath[MAX_PATH];
  snprintf( szRegPath, sizeof(szRegPath), "SYSTEM\\CurrentControlSet\\Services\\EventLog\\System\\%s", lpSourceName); //TODO: this may need further adjustments.
  if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, szRegPath, 0, KEY_READ, &hKey) != ERROR_SUCCESS )
  {
    printf("Could not open reg key for event log source: %s\n", lpSourceName);
    return NULL;
  }

  if( RegQueryValueEx( hKey, "EventMessageFile", NULL, NULL, (LPBYTE) szMessageFile, &dwBufferSize ) != ERROR_SUCCESS )
  {
    printf("Could not query EventMessageFile for event log source: %s\n", lpSourceName);
    RegCloseKey( hKey );
    return NULL;
  }

  if( !ExpandEnvironmentStrings( szMessageFile, szExpandedMessageFile, sizeof( szExpandedMessageFile )))
  {
    printf("Could not expand environment strings in message file path: %s\n", szMessageFile );
    return NULL;
  }

  //Load the message DLL
  HMODULE hSource = LoadLibraryEx( szExpandedMessageFile, NULL, DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_IMAGE_RESOURCE );
  if( hSource == NULL )
  {
    printf("Could not load message resource: %s Error: %lu\n", szExpandedMessageFile, GetLastError());
    return NULL;
  }

  DWORD dwFormatResult = FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_FROM_HMODULE,
      hSource,
      dwMessageId,
      0,
      (LPWSTR)&pBuffer,
      0,
      (va_list*)lpStrings
      );
  if( dwFormatResult == 0)
  {
    printf("FormatMessage failed with error %lu\n", GetLastError());
  }

  //strcpy_s( pszBuffer, iBufferLen, pBuffer );
  SafeStringCopy( pszBuffer, iBufferLen, pBuffer );
  FreeLibrary( hSource );

  return pBuffer;
}
void readEventLog(LPCSTR lpSourceName)
{
  HANDLE hEventLog = OpenEventLog(NULL, lpSourceName);
  if( hEventLog == NULL )
  {
    printf("Could not open event log %s Error: %lu\n", lpSourceName, GetLastError());
    return;
  }

  DWORD dwRead, dwNeeded, dwBufferSize = 0x10000;
  BYTE *pBuffer = (BYTE*)HeapAlloc(GetProcessHeap(), 0, dwBufferSize);

  if( pBuffer == NULL )
  {
    printf("Memory Allocation Failed.\n");
    CloseEventLog(hEventLog);
    return;
  }

  while( ReadEventLog(hEventLog, EVENTLOG_SEQUENTIAL_READ | EVENTLOG_FORWARDS_READ, 0, pBuffer, dwBufferSize, &dwRead, &dwNeeded))
  {
    printf("1");
    EVENTLOGRECORD *pRecord = (EVENTLOGRECORD *)pBuffer;
    while( (BYTE*)pRecord < pBuffer + dwRead)
    {
      time_t rawtime = pRecord->TimeGenerated;
      struct tm *ptm = gmtime(&rawtime);
      printf("TimeGenerated: %ld (UTC)\n", (long) rawtime);
      printf("EventID: %u\n", pRecord->EventID);
      printf("EventType: %u\n", pRecord->EventType);
      printf("Source: %s\n", (char*)pRecord + sizeof( EVENTLOGRECORD ));
      //Messages:
      LPVOID *lpStrings = (LPVOID *)HeapAlloc(GetProcessHeap(), 0, pRecord->NumStrings * sizeof(LPVOID));
      if( lpStrings == NULL )
      {
        printf("Memory allocation failed for strings.\n");
        HeapFree(GetProcessHeap(), 0, pBuffer);
        CloseEventLog(hEventLog);
        return;
      }

      //Collect all strings in message/description
      char *pString = (char*)pRecord + pRecord->StringOffset;
      for( int i = 0; i < pRecord->NumStrings; ++i )
      {
        lpStrings[i] = pString;
        pString + strlen(pString) + 1;
      }

      //Get formatted messages
      LPWSTR pMessage = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 4096 );
      GetFormattedMessage( pRecord->EventID, (char*)pRecord + sizeof( EVENTLOGRECORD ), lpStrings, pRecord->NumStrings, pMessage, 4096 );
      if( pMessage != NULL )
      {
        printf( "Message: %s\n", pMessage );
      }
      HeapFree( GetProcessHeap(), 0, pMessage );
      HeapFree( GetProcessHeap(), 0, lpStrings);
      printf("\n");
      
      //End messages
      pRecord = (EVENTLOGRECORD *)((BYTE *)pRecord + pRecord->Length);
    }
  }

  if( GetLastError() != ERROR_HANDLE_EOF )
  {
    printf("Error reading the event log: %lu\n", GetLastError());
  }

  HeapFree(GetProcessHeap(), 0, pBuffer);
  CloseEventLog(hEventLog);
}

int main()
{
  readEventLog("System");
  return 0;
}