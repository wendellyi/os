/*
 * wenix/include/assert.h
 */
 
#define assert(statment) do { if (0 == (statment)) panic(); } while (0)