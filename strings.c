#include "strings.h"
#include <stdio.h>
#include <string.h>

#define ZEROASCII 48
#define NINEASCII 57
#define AASCII 65
#define FASCII 70
#define aASCII 97
#define fASCII 102
#define xASCII 120
#define XASCII 88 
#define DECIMALBASE 10
#define HEXBASE 16

static unsigned int isValidHexDigit(char ch);
static unsigned int strtonum_base10(const char* str, const char** endptr);
static unsigned int strtonum_base16(const char* str, const char** endptr);
static unsigned int compute_num(int i, int length, const char *str, const char **endptr, unsigned int base);
static void bypass_leading_zeros(int i, const char *str);
static unsigned int offset(unsigned int ch);

/*  This function writes n bytes of c to the memory location pointed to by s */
void *memset(void *s, int c, size_t n)
{
    char* dst = (char*)s;   // need to make a copy of s
    for (; n > 0; n--) {
        *dst++ = (char)c;   // the value pointed to by dst = c; and then dst is incremented
    }

    return s;
}

/*  This function copies n bytes from mem location src to mem location dst. If dst and src overlap,
 *  behaviour is undefined */
void *memcpy(void *dst, const void *src, size_t n)
{
    // Type cast both dst and src
    char* dest = (char*)dst;
    char* source = (char*)src;

    // Copy over each index
    for (size_t i = 0; i < n; i++) {
        dest[i] = source[i];
    }

    return dst;
}

/*  This function returns the length of a string */
int strslen(const char *s)
{
    /* Implementation a gift to you from lab3 */
    int i;
    for (i = 0; s[i] != '\0'; i++) ;
    return i;
}

/*  This function compares two strings, index by index. Will return a 0 is the strings are the same,
 *  will return a -1 if the first word comes before the second, and will return a 1 if the second
 *  word comes before the first. */
int strcmp(const char *s1, const char *s2) {

    const char* s1i = s1;     // create a mutable s1 (s1i will iterate through s1)
    const char* s2i = s2;
    int i;

    for (i = 0; s1i[i] != '\0' && s2i[i] != '\0'; i++) {
        char letter1 = (s1i[i]);
        char letter2 = (s2i[i]);

        // If at any point one letter is greater than the other, then we know which string is greater
        if (letter1 > letter2) {
            return 1;
        } else if (letter2 > letter1) {
            return -1;
        }
    }

    // Will only reach this point if one string exists as the first part of another string (then the
    // shorter string is greatest) or the strings are the same
    if (s2i[i] != '\0') {
        return -1;
    } else if (s1i[i] != '\0') {
        return 1;
    } else {
        return 0;
    }
}

/*  This function appends the null-terminated str src to the end of dst. Appends at most 'maxsize -
 *  strlen(dst) - 1 bytes, and null-terminates the result */
int strlcat(char *dst, const char *src, int maxsize)
{
    const char* srci = src;
    int dLen = strlen(dst);
    int j;

    for (j = dLen; j < (maxsize - 1) && (j - dLen) < strlen(src); j++) {
        dst[j] = srci[j - dLen]; 
    }

    // Must null-terminate the string
    dst[j] = '\0';

    // Return j which is equal to the original length of dst + src
    return j;
} 

/*  This function converts a string to an integer when valid. Only decimal and hexedecimal bases
 *  supported. */
unsigned int strtonum(const char *str, const char **endptr) {

    // Need to differentiate between an entirely invalid str (starts with a letter or symbol), a hexedecimal
    // number, and a decimal number

    if (str[0] < ZEROASCII || str[0] > NINEASCII) {
        *endptr = &str[0];
        return -1;
    } else if (strlen(str) > 1 && str[0] == ZEROASCII && (str[1] == xASCII || str[1] == XASCII)) { 
        return strtonum_base16(str, endptr);
    } else {
        return strtonum_base10(str, endptr);
    }

}

static unsigned int strtonum_base10(const char *str, const char** endptr) {

    int i = 0, length;
    unsigned int base = 10;

    // Iterate through str to find the first invalid character (if it exists)
    for (length = 0; str[length] >= ZEROASCII && str[length] <= NINEASCII && length < strlen(str); length++) {}

    // length is now the new "string length" (anything after the invalid char is also invalid)

    // Need to iterate through any leading zeros
    bypass_leading_zeros(i, str);

    // Call compute_num to get the result
    return compute_num(i, length, str, endptr, base);
}

static unsigned int strtonum_base16(const char* str, const char** endptr) {

    // The iterator i will start at 2 because '0x' must be inputted
    int i = 2, length;
    unsigned int base = 16;

    // Iterate through str to find the first invalid character (if it exits)
    for (length = i; (isValidHexDigit(str[length])) && length < strlen(str); length++) {}
    
    // length is now the new "string length" (anything after the invalid char is also invalid)

    // Trim leading 0s
    bypass_leading_zeros(i, str);

    return compute_num(i, length, str, endptr, base);
}

/*  This function will return the number computed from a string (with the provided base) */
static unsigned int compute_num(int i, int length, const char *str, const char **endptr, unsigned int base) {

    // Initialize to 0
    unsigned int num = 0;
    unsigned int digit = 0;
    
    for ( ; i < length; i++) {
        
        // Retieve the offset for corresponding ASCII value
        unsigned int num_offset = offset(str[i]);
        
        if (!num_offset) {
            // This should never occur as we have already checked for invalid digits
            // however the redundancy exists to ensure the stability of the program
            *endptr = &str[i];
            return 0;    
        }

        digit = str[i] - num_offset;    // Find digit from ASCII offset

        // If it it the last valid digit, just add it; otherwise we must multiply it by its
        // corresponding power of 10 (which depends on the digit placement)
        if (i == (length - 1)) {
            num += digit;
        } else {
            int k = length - 1 - i;
            int place = 1;
            // Find the digit placement
            for (; k > 0; k--) {
                place *= base;
            }
            
            // Add the digit in the correct place to num
            num += place * digit;
        }
    }

    // if endptr is not NULL, it will point to the first invalid char of str 
    *endptr = &str[length];

    // return the resulting number (unsigned int)
    return num;
}

/*  This function finds and returns the ASCII offset */
static unsigned int offset(unsigned int ch) {
    if (ch >= ZEROASCII && ch <= NINEASCII) {
        return 48;
    } else if (ch >= AASCII && ch <= FASCII) {
        return 55;
    } else if (ch >= aASCII && ch <= fASCII) {
        return 87;
    } else {
        // Should never occur but is returned in the event an invalid char is passed
        // to offset()
        return 0;
    }
}

/*  This function returns a 1 if the ch is a valid hex digit, and returns 0 otherwise. */
static unsigned int isValidHexDigit(char ch) {

    if ((ch >= ZEROASCII && ch <= NINEASCII) || (ch >= AASCII && ch <= FASCII) || (ch >= aASCII && ch <= fASCII)) {
        return 1;
    }

    return 0;
}

/*  This function will parse through the string and iterate through all zeros. */
static void bypass_leading_zeros(int i, const char *str) {

    // Need to iterate through any leading zeros
    for ( ; str[i] == ZEROASCII && i < strlen(str); i++) {}
}

