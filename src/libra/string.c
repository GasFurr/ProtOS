#include "string.h"

void *memcpy(void *dest, const void *src, size_t n) {
  uint8_t *d = (uint8_t *)dest;
  const uint8_t *s = (const uint8_t *)src;
  for (size_t i = 0; i < n; i++)
    d[i] = s[i];
  return dest;
}

void *memset(void *s, int c, size_t n) {
  uint8_t *p = (uint8_t *)s;
  while (n--)
    *p++ = (uint8_t)c;
  return s;
}

void *memmove(void *dest, const void *src, size_t n) {
  uint8_t *d = (uint8_t *)dest;
  const uint8_t *s = (const uint8_t *)src;

  if (d < s) {
    for (size_t i = 0; i < n; i++)
      d[i] = s[i];
  } else {
    for (size_t i = n; i > 0; i--)
      d[i - 1] = s[i - 1];
  }
  return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const uint8_t *p1 = (const uint8_t *)s1;
  const uint8_t *p2 = (const uint8_t *)s2;
  for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i])
      return p1[i] - p2[i];
  }
  return 0;
}

size_t strlen(const char *s) {
  size_t len = 0;
  while (*s++)
    len++;
  return len;
}

char *strcpy(char *dest, const char *src) {
  char *orig = dest;
  while ((*dest++ = *src++))
    ;
  return orig;
}

char *strncpy(char *dest, const char *src, size_t n) {
  char *orig = dest;
  while (n-- && (*dest++ = *src++))
    ;
  while (n-- > 0)
    *dest++ = '\0';
  return orig;
}

int strcmp(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(const uint8_t *)s1 - *(const uint8_t *)s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  while (n-- && *s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return n == (size_t)-1 ? 0 : *(const uint8_t *)s1 - *(const uint8_t *)s2;
}

char *strcat(char *dest, const char *src) {
  char *orig = dest;
  dest += strlen(dest);
  strcpy(dest, src);
  return orig;
}

char *strncat(char *dest, const char *src, size_t n) {
  char *orig = dest;
  dest += strlen(dest);
  while (n-- && (*dest++ = *src++))
    ;
  *dest = '\0';
  return orig;
}
