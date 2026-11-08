#include "core_string.h"

char *StrCleanSpaces(const char *txt)
{
    if (!txt) return NULL;
    
    size_t jsonLen = strlen(txt);
    
    // Saltar BOM UTF-8 (EF BB BF) si existe
    const char *src = txt;
    size_t srcLen = jsonLen;
    
    if (srcLen >= 3 &&
        (unsigned char)src[0] == 0xEF &&
        (unsigned char)src[1] == 0xBB &&
        (unsigned char)src[2] == 0xBF)
    {
        src += 3;
        srcLen -= 3;
    }
    
    // Alocar buffer de salida
    char *result = (char *)malloc(srcLen + 1);
    if (!result) return NULL;
    
    // Copiar eliminando espacios SOLO al final de las keys
    char *dst = result;
    const char *p = src;
    int inString = 0;
    int spacesPending = 0;
    
    while (*p)
    {
        unsigned char c = (unsigned char)*p;
        
        if (c == '"')
        {
            // Verificar si la comilla está escapada
            int backslashes = 0;
            const char *check = p - 1;
            while (check >= src && *check == '\\')
            {
                backslashes++;
                check--;
            }
            
            if (backslashes % 2 == 0)
            {
                if (!inString)
                {
                    // Abriendo string
                    inString = 1;
                    spacesPending = 0;
                    *dst++ = c;
                }
                else
                {
                    // Cerrando string: ¿es KEY o VALUE?
                    inString = 0;
                    
                    // MIRAR ADELANTE para determinar si es key
                    const char *next = p + 1;
                    while (*next == ' ' || *next == '\t' || *next == '\r' || *next == '\n')
                        next++;
                    int isKey = (*next == ':') ? 1 : 0;
                    
                    if (isKey)
                    {
                        // Es KEY: descartar espacios pendientes
                        spacesPending = 0;
                    }
                    else
                    {
                        // Es VALUE: preservar espacios pendientes
                        while (spacesPending > 0) { *dst++ = ' '; spacesPending--; }
                    }
                    
                    *dst++ = c;
                }
            }
            else
            {
                // Comilla escapada: escribir espacios pendientes + la comilla
                while (spacesPending > 0) { *dst++ = ' '; spacesPending--; }
                *dst++ = c;
            }
        }
        else if (inString && (c == ' ' || c == '\t'))
        {
            // Espacio dentro de string: acumular como pendiente
            spacesPending++;
        }
        else
        {
            // Cualquier otro caracter: escribir espacios pendientes + el caracter
            if (inString)
            {
                while (spacesPending > 0) { *dst++ = ' '; spacesPending--; }
            }
            *dst++ = c;
        }
        
        p++;
    }
    
    *dst = '\0';
    return result;
} 

char* StrRemoveChar(char* str, char c) {
    if (!str || c == '\0') {
        return str; 
    }
    char* src = str;
    char* dst = str;
    while (*src != '\0') {
        if (*src != c) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
    return str;
}

void StrAllTrim(char *str)
{
    if (!str)
        return;

    // Caso borde: string vacío -> evita underflow en strlen()-1
    if (*str == '\0')
        return;

    // Trim leading
    char *start = str;
    while (*start == ' ' || *start == '\t' || *start == '\r' || *start == '\n')
        start++;

    // Si el string era todo espacios, start ahora apunta al '\0' final
    if (*start == '\0')
    {
        *str = '\0';
        return;
    }

    // Trim trailing
    char *end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
        end--;
    *(end + 1) = '\0';

    // Move to beginning
    if (start != str)
        memmove(str, start, strlen(start) + 1);
}

int StrCompare(const char *str1, const char *str2)
{
    return str_compare(str1, str2);
}
int StrNCompare(const char *str1, const char *str2, size_t MaxCount)
{
    return str_n_compare(str1, str2, MaxCount);
}

char *StrLeftSafe(char *dest, size_t destsize, const char *src, size_t n)
{
    if (dest == NULL || src == NULL || destsize == 0)
        return dest;

    if (n >= destsize)
    {
        n = destsize - 1;
    }

    size_t len = strlen(src);
    if (n > len)
        n = len;

    memcpy(dest, src, n);
    dest[n] = '\0';
    return dest;
}

char *StrLeft(char *dest, const char *src, size_t n)
{
    if (dest == NULL || src == NULL)
        return dest;
    size_t len = strlen(src);
    if (n > len)
        n = len;

    memcpy(dest, src, n);
    dest[n] = '\0';
    return dest;
}

char *StrRight(char *dest, const char *src, size_t n)
{
    if (dest == NULL || src == NULL)
        return dest;
    size_t len = strlen(src);
    if (n > len)
        n = len;

    memcpy(dest, src + (len - n), n);
    dest[n] = '\0';
    return dest;
}

// ============================================================================
// PADDING - versiones originales (asumen buffer ya dimensionado por el caller)
// ============================================================================

char *StrPadL(char *dest, const char *src, size_t totallen, char padchar)
{
    if (dest == NULL || src == NULL)
        return dest;
    size_t srclen = strlen(src);

    if (srclen >= totallen)
    {
        return StrLeft(dest, src, totallen);
    }

    size_t numpads = totallen - srclen;
    memset(dest, padchar, numpads);
    memcpy(dest + numpads, src, srclen);
    dest[totallen] = '\0';
    return dest;
}

char *StrPadR(char *dest, const char *src, size_t totallen, char padchar)
{
    if (dest == NULL || src == NULL)
        return dest;
    size_t srclen = strlen(src);

    if (srclen >= totallen)
    {
        return StrLeft(dest, src, totallen);
    }
    memcpy(dest, src, srclen);
    memset(dest + srclen, padchar, totallen - srclen);
    dest[totallen] = '\0';
    return dest;
}

char *StrPadLSafe(char *dest, size_t destsize, const char *src, size_t totallen, char padchar)
{
    if (dest == NULL || src == NULL || destsize == 0)
        return dest;

    // Clamp totallen para que siempre entre el '\0'
    if (totallen >= destsize)
        totallen = destsize - 1;

    return StrPadL(dest, src, totallen, padchar);
}

char *StrPadRSafe(char *dest, size_t destsize, const char *src, size_t totallen, char padchar)
{
    if (dest == NULL || src == NULL || destsize == 0)
        return dest;

    if (totallen >= destsize)
        totallen = destsize - 1;

    return StrPadR(dest, src, totallen, padchar);
}

// ============================================================================
// NORMALIZACIÓN / TRIM / CASE
// ============================================================================

void StrNormalize(char *dest, const char *src)
{
    if (dest == NULL || src == NULL)
        return;

    while (*src && isspace((unsigned char)*src))
    {
        src++;
    }
    size_t len = strlen(src);
    while (len > 0 && isspace((unsigned char)src[len - 1]))
    {
        len--;
    }
    for (size_t i = 0; i < len; i++)
    {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[len] = '\0';
}

char *StrTrimAllAlloc(const char *src) {
    if (src == NULL) return NULL;
    while (*src && isspace((unsigned char)*src)) {
        src++;
    }
    size_t len = strlen(src);
    while (len > 0 && isspace((unsigned char)src[len - 1])) {
        len--;
    }
    char *dest = (char *)malloc(len + 1);
    if (dest != NULL) {
        memcpy(dest, src, len);
        dest[len] = '\0';
    }
    return dest; 
}

char *StrTrimAll(char *dest, const char *src)
{
    if (dest == NULL || src == NULL)
        return dest;

    while (*src && isspace((unsigned char)*src))
    {
        src++;
    }

    size_t len = strlen(src);
    while (len > 0 && isspace((unsigned char)src[len - 1]))
    {
        len--;
    }

    memcpy(dest, src, len);
    dest[len] = '\0';

    return dest;
}

char *StrToLower(char *dest, const char *src)
{
    if (dest == NULL || src == NULL)
        return dest;
    size_t len = strlen(src);
    for (size_t i = 0; i < len; i++)
    {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[len] = '\0';
    return dest;
}

char *StrToUpper(char *dest, const char *src)
{
    if (dest == NULL || src == NULL)
        return dest;
    size_t len = strlen(src);
    for (size_t i = 0; i < len; i++)
    {
        dest[i] = (char)toupper((unsigned char)src[i]);
    }
    dest[len] = '\0';
    return dest;
}

// Versiones "Safe" con destsize explícito para Normalize/TrimAll/ToLower/ToUpper.
// Truncan el resultado si no entra completo en destsize.
char *StrTrimAllSafe(char *dest, size_t destsize, const char *src)
{
    if (dest == NULL || src == NULL || destsize == 0)
        return dest;

    while (*src && isspace((unsigned char)*src))
        src++;

    size_t len = strlen(src);
    while (len > 0 && isspace((unsigned char)src[len - 1]))
        len--;

    if (len >= destsize)
        len = destsize - 1;

    memcpy(dest, src, len);
    dest[len] = '\0';
    return dest;
}

char *StrToLowerSafe(char *dest, size_t destsize, const char *src)
{
    if (dest == NULL || src == NULL || destsize == 0)
        return dest;

    size_t len = strlen(src);
    if (len >= destsize)
        len = destsize - 1;

    for (size_t i = 0; i < len; i++)
        dest[i] = (char)tolower((unsigned char)src[i]);
    dest[len] = '\0';
    return dest;
}

char *StrToUpperSafe(char *dest, size_t destsize, const char *src)
{
    if (dest == NULL || src == NULL || destsize == 0)
        return dest;

    size_t len = strlen(src);
    if (len >= destsize)
        len = destsize - 1;

    for (size_t i = 0; i < len; i++)
        dest[i] = (char)toupper((unsigned char)src[i]);
    dest[len] = '\0';
    return dest;
}

// ============================================================================
// ESCAPE / UNESCAPE (comillas simples para SQL embebido)
// ============================================================================

char *StrEscapeOut(const char *input, int len, char *output, size_t outputsize)
{
    if (input == NULL || output == NULL || len <= 0 || outputsize == 0)
    {
        if (output && outputsize > 0)
            output[0] = '\0';
        return NULL;
    }

    int i = 0;
    size_t j = 0;

    // Reservamos 1 byte para el '\0' final: escribimos como máximo outputsize - 1
    while (i < len)
    {
        if (input[i] == '\'')
        {
            if (j + 2 > outputsize - 1)
            {
                // No entra el par de comillas escapadas + terminador -> abortar
                output[j] = '\0';
                return NULL;
            }
            output[j++] = '\'';
            output[j++] = '\'';
            i++;
        }
        else
        {
            if (j + 1 > outputsize - 1)
            {
                output[j] = '\0';
                return NULL;
            }
            output[j++] = input[i++];
        }
    }
    output[j] = '\0';
    return output;
}

char *StrUnescape(const char *input, int len)
{
    if (input == NULL || len <= 0)
    {
        return NULL;
    }
    char *output = (char *)malloc((size_t)len + 1);
    if (output == NULL)
    {
        return NULL;
    }
    int i = 0;
    int j = 0;

    while (i < len)
    {
        if (input[i] == '\'' && (i + 1 < len) && input[i + 1] == '\'')
        {
            output[j++] = '\'';
            i += 2;
        }
        else
        {
            output[j++] = input[i++];
        }
    }
    output[j] = '\0';
    return output;
}
 

char *StrFormat(const char *formato, ...)
{
    if (formato == NULL)
        return NULL;

    va_list args1;
    va_list args2;

    va_start(args1, formato);
    va_copy(args2, args1);

    int longitud = vsnprintf(NULL, 0, formato, args1);
    va_end(args1);

    if (longitud < 0)
    {
        va_end(args2);
        return NULL;
    }

    char *resultado = (char *)malloc((size_t)longitud + 1);
    if (resultado == NULL)
    {
        va_end(args2);
        return NULL;
    }

    vsnprintf(resultado, (size_t)longitud + 1, formato, args2);
    va_end(args2);

    return resultado;
}

char *StrSanitize(const char *input)
{
    if (!input)
        return _strdup("");

    size_t len = strlen(input);
    // Peor caso: cada caracter es una comilla simple → se duplica el tamaño
    char *out = (char *)malloc(len * 2 + 1);
    if (!out)
        return _strdup("");

    size_t j = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (input[i] == '\'')
        {
            out[j++] = '\''; // VFP escapa comillas duplicándolas: ' → ''
            out[j++] = '\'';
        }
        else if (input[i] == '\0')
        {
            break; // Proteger contra strings con nulos embebidos
        }
        else
        {
            out[j++] = input[i];
        }
    }
    out[j] = '\0';
    return out;
}
void StrTranChar(char *str, char find, char replace) {
    if (!str) return;
    
    while (*str != '\0') {
        if (*str == find) {
            *str = replace;
        }
        str++;
    }
}
void StrTranCharW(wchar_t *str, wchar_t find, wchar_t replace) 
{ 
    if (!str) return; 
    
    while (*str != L'\0') 
    { 
        if (*str == find) 
        { 
            *str = replace; 
        } 
        str++; 
    } 
}

char* StrIn(
    const char* strBegin,
    const char* strEnd,
    const char* strSource,
    bool includeBegin,
    bool includeEnd,
    char* outBuffer,
    size_t bufferSize) {
    
    if (!strBegin || !strEnd || !strSource || !outBuffer || bufferSize == 0) {
        if (outBuffer && bufferSize > 0) {
            outBuffer[0] = '\0';
        }
        return NULL;
    }

    outBuffer[0] = '\0';

    const char* pBegin = strstr(strSource, strBegin);
    if (pBegin != NULL) {
        
        if (!includeBegin) {
            pBegin += strlen(strBegin);
        }

        const char* searchStart = pBegin;
        if (includeBegin) {
            searchStart += strlen(strBegin);
        }

        const char* pEnd = strstr(searchStart, strEnd);
        if (pEnd != NULL) {
            
            if (includeEnd) {
                pEnd += strlen(strEnd);
            }

            size_t length = pEnd - pBegin;

            if (length >= bufferSize) {
                length = bufferSize - 1;
            }

            strncpy(outBuffer, pBegin, length);
            outBuffer[length] = '\0';
        }
    }
    return outBuffer;
}