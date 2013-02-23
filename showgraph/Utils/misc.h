/**
 * @file: misc.h 
 * @defgroup Misc Miscellaneous
 * @brief Various auxiliary routines
 * @ingroup Utils
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef MISC_H
#define MISC_H

/**
 * Check if pointer is not null
 * @ingroup Misc
 */
inline bool isNotNullP( const void *pointer)
{
    return pointer != NULL;
}
/**
 * Check if pointer is null
 * @ingroup Misc
 */
inline bool isNullP( const void *pointer)
{
    return pointer == NULL;
}

/**
 * Check if pointers are equal
 * @ingroup Misc
 */
inline bool areEqP( const void *p1, const void *p2)
{
    return p1 == p2;
}

/**
 * Check if pointers are not equal
 * @ingroup Misc
 */
inline bool areNotEqP( const void *p1, const void *p2)
{
    return p1 != p2;
}

/**
 * @brief Get absolute value
 * @ingroup Misc
 * @param Value value type
 * @param val signed value used to get absolute value
 *
 * @par 
 * Returns absolute value for a given signed operand. Note that operators
 * "<" and "-" must be defined for parameter class
 */
template<class Value> inline Value abs( Value val)
{
    if( val < 0)
        return -val;

    return val;
}

#endif