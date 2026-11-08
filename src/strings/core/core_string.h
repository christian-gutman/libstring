#ifndef CORE_STRING_H
#define CORE_STRING_H
 
#include "core_encoding.h"

#include <locale.h>
#include <strsafe.h>

#ifdef __cplusplus
extern "C"
{
#endif

    CORE_API void StrAllTrim(char *str);

    CORE_API int StrCompare(const char *str1, const char *str2);

    CORE_API int StrNCompare(const char *str1, const char *str2, size_t MaxCount);

    CORE_API char *StrLeftSafe(char *dest, size_t destsize, const char *src, size_t n);

    CORE_API char *StrLeft(char *dest, const char *src, size_t n);

    CORE_API char *StrRight(char *dest, const char *src, size_t n);

    CORE_API char *StrPadL(char *dest, const char *src, size_t totallen, char padchar);

    CORE_API char *StrPadR(char *dest, const char *src, size_t totallen, char padchar);

    CORE_API char *StrPadLSafe(char *dest, size_t destsize, const char *src, size_t totallen, char padchar);
    CORE_API char *StrPadRSafe(char *dest, size_t destsize, const char *src, size_t totallen, char padchar);

    CORE_API void StrNormalize(char *dest, const char *src);

    CORE_API char *StrTrimAll(char *dest, const char *src);

    CORE_API char *StrTrimAllAlloc(const char *src);

    CORE_API char *StrToLower(char *dest, const char *src);

    CORE_API char *StrToUpper(char *dest, const char *src);

    CORE_API char *StrTrimAllSafe(char *dest, size_t destsize, const char *src);
    CORE_API char *StrToLowerSafe(char *dest, size_t destsize, const char *src);
    CORE_API char *StrToUpperSafe(char *dest, size_t destsize, const char *src);

    CORE_API char *StrEscapeOut(const char *input, int len, char *output, size_t outputsize);

    //CORE_API char *StrEscape(const char *input, int len, char *output);

    CORE_API char *StrUnescape(const char *input, int len);

    CORE_API char *StrFormat(const char *formato, ...);

    CORE_API char *StrSanitize(const char *input);

    CORE_API char *StrRemoveChar(char *str, char c);

    CORE_API char *StrCleanSpaces(const char *txt);

    CORE_API void StrTranChar(char *str, char find, char replace);
    CORE_API void StrTranCharW(wchar_t *str, wchar_t find, wchar_t replace);

    CORE_API char *StrIn(
        const char *strBegin,
        const char *strEnd,
        const char *strSource,
        bool includeBegin,
        bool includeEnd,
        char *outBuffer,
        size_t bufferSize);

#ifdef __cplusplus
}
#endif

#endif // CORE_STRING_H
