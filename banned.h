/***
* banned.h - list of APIs banned in the Secure Software Development Lifecycle.
*
* Purpose: 
*	This include file contains a list of banned APIs which should not be used in new code
*	and removed from legacy code.
*
* Note:
* Using this file doesn't avoid mistakes by developers, it only seeks to provide APIs which
* are more explicit and assist in making more secure decisions.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
***/

#ifndef _INC_BANNED
#define _INC_BANNED

// Offer functions which are more difficult to use insecurely:

// Additional return codes tracking those produced by Microsoft for consistency:
#ifndef SUCCEEDED
  #define SUCCEEDED(hr) ((int)(hr) >= 0)
#endif
#ifndef FAILED
  #define FAILED(hr) ((int)(hr) < 0)
#endif
#define STRSAFE_E_INSUFFICIENT_BUFFER ((int)0x8007007A)
#define STRSAFE_E_INVALID_PARAMETER ((int)0x80070057)
#define STRSAFE_E_END_OF_FILE ((int)0x80070026)
#ifndef S_OK
  #define S_OK ((int)0x00000000)
#endif
#define STRSAFE_MAX_CCH 2147483647

// modeled after StringCopyWorkerA
int strcpy_worker_a_strsafe(char* pszDest,size_t cchDest,const char* pszSrc) {
  int hr = S_OK;
  if (cchDest==0) {
    hr = STRSAFE_E_INVALID_PARAMETER;
  } else {
    while(cchDest && (*pszSrc!='\0')) {
      *pszDest++ = *pszSrc++;
      cchDest--;
    }
    if(cchDest==0) {
      pszDest--;
      hr = STRSAFE_E_INSUFFICIENT_BUFFER;
    }
    *pszDest= '\0'; //always terminate with a null byte, even if truncated
  }
  return hr;
}

// modeled after StringCbCopyA
int strncpy_strsafe(char* pszDest,size_t cbDest,const char* pszSrc) {
  if(cbDest > STRSAFE_MAX_CCH) return STRSAFE_E_INVALID_PARAMETER;
  return strcpy_worker_a_strsafe(pszDest,cbDest,pszSrc);
}

// modeled after StringLengthWorkerA
int strlen_worker_a_strsafe(char* psz, size_t cchMax, size_t *pcchLength) {
  int hr = S_OK;
  size_t cchMaxPrev = cchMax;
  while (cchMax && (*psz!='\0')) {
    psz++;
    cchMax--;
  }
  if (cchMax == 0) {
    hr = STRSAFE_E_INVALID_PARAMETER;
  }
  if (pcchLength) {
    if(SUCCEEDED(hr)) {
      *pcchLength = cchMaxPrev - cchMax;
    } else {
      *pcchLength = 0;
    }
  }
  return hr;
}

// modeled after StringCatWorkerA
int strcat_worker_a_strsafe(char* pszDest, size_t cchDest, char* pszSrc) {
  int hr;
  size_t cchDestLength;
  hr = strlen_worker_a_strsafe(pszDest, cchDest, &cchDestLength);
  if(SUCCEEDED(hr)) {
    hr = strcpy_worker_a_strsafe(pszDest + cchDestLength, cchDest - cchDestLength, pszSrc);
  }
  return hr;
}

// modeled after StringCchCatA/StringCbCatA
int strcat_strsafe(char* pszDest, size_t cchDest, char* pszSrc) {
  if (cchDest > STRSAFE_MAX_CCH) {
    return STRSAFE_E_INVALID_PARAMETER;
  }
  return strcat_worker_a_strsafe(pszDest, cchDest, pszSrc);
}

// modeled after StringCopyNWorkerA
int strncpy_worker_a_strsafe(char* pszDest, size_t cchDest, char* pszSrc, size_t cchSrc) {
  int hr = S_OK;
  if (cchDest==0) {
    hr = STRSAFE_E_INVALID_PARAMETER;
  } else {
    while (cchDest && cchSrc && (*pszSrc!='\0')) {
      *pszDest++ = *pszSrc++;
      cchDest--;
      cchSrc--;
    }
    if (cchDest==0) {
      pszDest--;
      hr = STRSAFE_E_INSUFFICIENT_BUFFER;
    }
    *pszDest= '\0';
  }
  return hr;
}

// modeled after StringCatNWorkerA
int strncat_worker_a_strsafe(char* pszDest, size_t cchDest, char* pszSrc, size_t cchToAppend) {
  int hr;
  size_t cchDestLength;
  hr = strlen_worker_a_strsafe(pszDest,cchDest,&cchDestLength);
  if (SUCCEEDED(hr)) {
    hr = strncpy_worker_a_strsafe(pszDest + cchDestLength,cchDest - cchDestLength,pszSrc,cchToAppend);
  }
  return hr;
}

// modeled after StringCchCatNA
int strncat_strsafe(char* pszDest, size_t cchDest, char* pszSrc, size_t cchToAppend) {
  if(cchDest > STRSAFE_MAX_CCH) {
    return STRSAFE_E_INVALID_PARAMETER;
  }
  return strncat_worker_a_strsafe(pszDest, cchDest, pszSrc, cchToAppend);
}

// Insert strncat_strsafe items modeled from StringCbCatA and StringCchCatA

#ifdef wchar_t //in case wchar_t isn't defined
    // modeled after StringCopyWorkerW
    int strcpy_worker_w_strsafe(wchar_t* pszDest,size_t cchDest,const wchar_t* pszSrc) {
        int hr = S_OK;
        if (cchDest==0) {
            hr = STRSAFE_E_INVALID_PARAMETER;
        } else {
            while(cchDest && (*pszSrc!=L'\0')) {
            *pszDest++ = *pszSrc++;
            cchDest--;
            }
            if(cchDest==0) {
            pszDest--;
            hr = STRSAFE_E_INSUFFICIENT_BUFFER;
            }
            *pszDest= L'\0'; //always terminate with a null byte, even if truncated
        }
        return hr;
    }

    // modeled after StringCbCopyW
    // cbDest: The size of the destination buffer, in bytes
    int wcscpy_bytes_strsafe(wchar_t* pszDest,size_t cbDest,const wchar_t* pszSrc) {
        size_t cchDest = cbDest / sizeof(wchar_t);
        if(cchDest > STRSAFE_MAX_CCH) return STRSAFE_E_INVALID_PARAMETER;
        return strcpy_worker_w_strsafe(pszDest,cchDest,pszSrc);
    }

    // modeled after StringCchCopyW
    // cchDest: The size of the destination buffer, in characters
    int wcscpy_chars_strsafe(wchar_t* pszDest,size_t cchDest,const wchar_t* pszSrc) {
        if(cchDest > STRSAFE_MAX_CCH) return STRSAFE_E_INVALID_PARAMETER;
        return strcpy_worker_w_strsafe(pszDest,cchDest,pszSrc);
    }

    // modeled after StringCatWorkerW
    int strcat_worker_w_strsafe(wchar_t* pszDest,size_t cchDest,wchar_t* pszSrc) {
      HRESULT hr;
      size_t cchDestLength;
      hr = StringLengthWorkerW(pszDest, cchDest, &cchDestLength);
      if (SUCCEEDED(hr)) {
        hr = strcpy_worker_w_strsafe(pszDest + cchDestLength, cchDest - cchDestLength, pszSrc);
      }
      return hr;
    }

    // modeled after StringCbCatW
    int wcscat_bytes_strsafe(wchar_t* pszDest,size_t cbDest,wchar_t* pszSrc) {
      size_t cchDest = cbDest / sizeof(wchar_t);
      if(cchDest > STRSAFE_MAX_CCH) {
        return STRSAFE_E_INVALID_PARAMETER;
      }
      return strcat_worker_w_strsafe(pszDest, cchDest, pszSrc);
    }

    // modeled after StringCchCatW
    int wcscat_chars_strsafe(wchar_t* pszDest, size_t cchDest, const wchar_t* pszSrc) {
      if (cchDest > STRSAFE_MAX_CCH) {
        return STRSAFE_E_INVALID_PARAMETER;
      }
      return strcat_worker_w_strsafe(pszDest, cchDest, pszSrc);
    }
#endif //wchar_t

// Mark risky functions as deprecated as a compiler warning ONLY
#if defined(EMBD_STRSAFE_WARN_ONLY)

#ifdef __clang__
// For gcc llvm clang, deprecated pragma isn't recognized:

#undef strcpy
char* strcpy(char*, const char*) __attribute__((deprecated("This function isn't considered secure.", "strncpy_strsafe or strlcpy")));
#undef strncpy
char* strncpy(char*, const char*, size_t) __attribute__((deprecated("This function isn't considered secure.", "strncpy_strsafe")));
#undef strcat
char* strcat(char *, const char *) __attribute__((deprecated("This function isn't considered secure.", "strcat_strsafe or strncat_strsafe")));
#undef strncat
char* strncat(char *, const char *, size_t) __attribute__((deprecated("This function isn't considered secure.", "strncat_strsafe")));
#undef sprintf
int sprintf(char *, const char *, ...) __attribute__((deprecated("This function isn't considered secure.", "snprintf")));
#undef vsprintf
int vsprintf(char *, const char *, va_list ap) __attribute__((deprecated("This function isn't considered secure.", "vsnprintf")));
#undef gets
char * gets(char *) __attribute__((deprecated("This function isn't considered secure.", "fgets")));
#ifdef wchar_t //in case wchar_t isn't defined
    #undef wcscpy
    wchar_t* wcscpy(wchar_t*, const wchar_t*) __attribute__((deprecated("This function isn't considered secure.", "wcscpy_chars_strsafe or wcscpy_bytes_strsafe")));
    #undef wcsncpy
    wchar_t* wcsncpy(wchar_t*, const wchar_t*, size_t n) __attribute__((deprecated("This function isn't considered secure.", "wcscpy_chars_strsafe or wcscpy_bytes_strsafe")));
    #undef wcscat
    wchar_t* wcscat(wchar_t *, const wchar_t *) __attribute__((deprecated("This function isn't considered secure.", "wcscat_chars_strsafe or wcscat_bytes_strsafe")));
    // TODO: Add strsafe function for wcsncat (not implemented in Microsoft version)
    #undef wcsncat
    wchar_t* wcsncat(wchar_t *, const wchar_t *, size_t) __attribute__((deprecated("This function isn't considered secure.", "Not yet implemented.")));
#endif //wchar_t
#undef alloca
void * alloca(size_t) __attribute__((deprecated("This function isn't considered secure.", "Remove")));

#else //__clang__
// If not compiling with clang, use the deprecated pragma:
#pragma deprecated (strcpy, wcscpy) //strlcpy is currently allowed as it should null terminate
#pragma deprecated (strncpy, wcsncpy)
#pragma deprecated (strcat, wcscat)
#pragma deprecated (strncat, wcsncat)
#pragma deprecated (sprintf) //swprintf is currently allowed as takes size info
#pragma deprecated (vsprintf) //vswprintf is currently allowed as takes size info
//#pragma deprecated (vsnprintf)
#pragma deprecated (gets)
// TODO: Decide if these will have alternate functions,
//       as the return code int vs length int could become confusing for a developer.
#pragma deprecated (strlen, wcslen) // TODO: add to clang handling
#pragma deprecated (memcpy, wmemcpy)
//#pragma deprecated (strtok, wcstok)
#pragma deprecated (scanf, wscanf, sscanf, swscanf) // TODO: add to clang handling, see notes on snscanf/snwscanf
#pragma deprecated (itoa, itow, ultoa, ultow) // TODO: add to clang handling
#pragma deprecated (alloca)
#endif //!__clang__

// Else, undefine all risky functions and provide a banned function error message during compilation.
// If you do not want this, you can #define EMBD_STRSAFE_WARN_ONLY before including this file.
#else //!EMBD_STRSAFE_WARN_ONLY

// NOTE: Many functions are included even if they are not POSIX out of an abundance of caution.
#define BANNED(func) Sorry_##func##_is_a_banned_function

#undef strcpy
#define strcpy BANNED(strcpy);

#undef strcpyA
#define strcpyA BANNED(strcpyA);

#undef strcpyW
#define strcpyW BANNED(strcpyW);

#undef wcscpy
#define wcscpy BANNED(wcscpy);

#undef lstrcpy
#define lstrcpy BANNED(lstrcpy);

#undef _tccpy
#define _tccpy BANNED(_tccpy);

#undef _mbccpy
#define _mbccpy BANNED(_mbccpy);

#undef _ftcscpy
#define _ftcscpy BANNED(_ftcscpy);

#undef strcat
#define strcat BANNED(strcat);

#undef strcatA
#define strcatA BANNED(strcatA);

#undef strcatW
#define strcatW BANNED(strcatW);

#undef wcscat
#define wcscat BANNED(wcscat);

#undef _tcscat
#define _tcscat BANNED(_tcscat);

#undef _mbscat
#define _mbscat BANNED(_mbscat);

#undef lstrcat
#define lstrcat BANNED(lstrcat);

#undef lstrcatA
#define lstrcatA BANNED(lstrcatA);

#undef lstrcatW
#define lstrcatW BANNED(lstrcatW);

#undef _tccat
#define _tccat BANNED(_tccat);

#undef _mbccat
#define _mbccat BANNED(_mbccat);

#undef _ftcscat
#define _ftcscat BANNED(_ftcscat);

#undef sprintfW
#define sprintfW BANNED(sprintfW);

#undef sprintfA
#define sprintfA BANNED(sprintfA);

#undef wsprintf
#define wsprintf BANNED(wsprintf);

#undef wsprintfW
#define wsprintfW BANNED(wsprintfW);

#undef wsprintfA
#define wsprintfA BANNED(wsprintfA);

#undef sprintf
#define sprintf BANNED(sprintf);

#undef swprintf
#define swprintf BANNED(swprintf);

#undef _stprintf
#define _stprintf BANNED(_stprintf);

#undef wvsprintf
#define wvsprintf BANNED(wvsprintf);

#undef vsprintf
#define vsprintf BANNED(vsprintf);

#undef _vstprintf
#define _vstprintf BANNED(_vstprintf);

#undef vswprintf
#define vswprintf BANNED(vswprintf);

#undef strncpy
#define strncpy BANNED(strncpy);

#undef wcsncpy
#define wcsncpy BANNED(wcsncpy);

#undef _tcsncpy
#define _tcsncpy BANNED(_tcsncpy);

#undef _mbsncpy
#define _mbsncpy BANNED(_mbsncpy);

#undef _mbsnbcpy
#define _mbsnbcpy BANNED(_mbsnbcpy);

#undef lstrcpyn
#define lstrcpyn BANNED(lstrcpyn);

#undef strncat
#define strncat BANNED(strncat);

#undef wcsncat
#define wcsncat BANNED(wcsncat);

#undef _tcsncat
#define _tcsncat BANNED(_tcsncat);

#undef _mbsncat
#define _mbsncat BANNED(_mbsncat);

#undef _mbsnbcat
#define _mbsnbcat BANNED(_mbsnbcat);

#undef lstrncat
#define lstrncat BANNED(lstrncat);

#undef lstrcatn
#define lstrcatn BANNED(lstrcatn);

#undef gets
#define gets BANNED(gets);

#undef _getts
#define _getts BANNED(_getts);

#undef _gettws
#define _gettws BANNED(_gettws);

#undef memcpy
#define memcpy BANNED(memcpy);

#undef wmemcpy
#define wmemcpy BANNED(wmemcpy);

#undef wnsprintf
#define wnsprintf BANNED(wnsprintf);

#undef wnsprintfA
#define wnsprintfA BANNED(wnsprintfA);

#undef wnsprintfW
#define wnsprintfW BANNED(wnsprintfW);

#undef _snwprintf
#define _snwprintf BANNED(_snwprintf);

#undef _snprintf
#define _snprintf BANNED(_snprintf);

#undef _sntprintf
#define _sntprintf BANNED(_sntprintf);

#undef _vsnprintf
#define _vsnprintf BANNED(_vsnprintf);

#undef vsnprintf
#define vsnprintf BANNED(vsnprintf);

#undef _vsnwprintf
#define _vsnwprintf BANNED(_vsnwprintf);

#undef _vsntprintf
#define _vsntprintf BANNED(_vsntprintf);

#undef wvnsprintf
#define wvnsprintf BANNED(wvnsprintf);

#undef strtok
#define strtok BANNED(strtok);

#undef _tcstok
#define _tcstok BANNED(_tcstok);

#undef _mbstok
#define _mbstok BANNED(_mbstok);

#undef makepath
#define makepath BANNED(makepath);

#undef _tmakepath
#define _tmakepath BANNED(_tmakepath);

#undef _makepath
#define _makepath BANNED(_makepath);

#undef _wmakepath
#define _wmakepath BANNED(_wmakepath);

#undef _splitpath
#define _splitpath BANNED(_splitpath);

#undef _tsplitpath
#define _tsplitpath BANNED(_tsplitpath);

#undef _wsplitpath
#define _wsplitpath BANNED(_wsplitpath);

#undef scanf
#define scanf BANNED(scanf);

#undef wscanf
#define wscanf BANNED(wscanf);

#undef _tscanf
#define _tscanf BANNED(_tscanf);

#undef sscanf
#define sscanf BANNED(sscanf);

#undef swscanf
#define swscanf BANNED(swscanf);

#undef _stscanf
#define _stscanf BANNED(_stscanf);

#undef snscanf
#define snscanf BANNED(snscanf);

#undef snscanf
#define snscanf BANNED(snscanf);

#undef snwscanf
#define snwscanf BANNED(snwscanf);

#undef _sntscanf
#define _sntscanf BANNED(_sntscanf);

#undef _itoa
#define _itoa BANNED(_itoa);

#undef _itow
#define _itow BANNED(_itow);

#undef _i64toa
#define _i64toa BANNED(_i64toa);

#undef _i64tow
#define _i64tow BANNED(_i64tow);

#undef _ui64toa
#define _ui64toa BANNED(_ui64toa);

#undef _ui64tot
#define _ui64tot BANNED(_ui64tot);

#undef _ui64tow
#define _ui64tow BANNED(_ui64tow);

#undef _ultoa
#define _ultoa BANNED(_ultoa);

#undef _ultot
#define _ultot BANNED(_ultot);

#undef _ultow
#define _ultow BANNED(_ultow);

#undef alloca
#define alloca BANNED(alloca);

#undef _alloca
#define _alloca BANNED(_alloca);

#undef strlen
#define strlen BANNED(strlen);

#undef wcslen
#define wcslen BANNED(wcslen);

#undef _mbslen
#define _mbslen BANNED(_mbslen);

#undef _mbstrlen
#define _mbstrlen BANNED(_mbstrlen);

#undef lstrlen
#define lstrlen BANNED(lstrlen);

#undef lstrlen
#define lstrlen BANNED(lstrlen);
#endif // EMBD_STRSAFE_WARN_ONLY
#endif // _INC_BANNED
