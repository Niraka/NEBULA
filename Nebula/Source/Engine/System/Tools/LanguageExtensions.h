/**
A collection of macro based language extensions.

@date edited 10/09/2016
@date authored 10/09 /2016

@author Nathan Sainsbury */

#ifndef LANGUAGE_EXTENSIONS_H
#define LANGUAGE_EXTENSIONS_H

/**
Evaluates to true when the result is not. */
#define ifnot(x) if (!(x))

/**
Evaluates to true when the given pointer is a nullptr. */
#define isnullptr(x) x == nullptr

/**
Evaluates to true when the given pointer is not a nullptr. */
#define notnullptr(x) x != nullptr

#endif