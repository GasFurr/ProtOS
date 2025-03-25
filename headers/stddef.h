#ifndef _STDDEF_H
#define _STDDEF_H

typedef unsigned int size_t;      // Size type (32-bit)
typedef int ptrdiff_t;            // Pointer difference type

#define NULL ((void*)0)           // Null pointer constant

// Get offset of member in struct
#define offsetof(TYPE, MEMBER) ((size_t)&((TYPE *)0)->MEMBER)

#endif /* _STDDEF_H */
