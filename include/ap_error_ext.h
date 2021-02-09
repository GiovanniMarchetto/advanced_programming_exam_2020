/**
 * This header adds new macros to the header file "ap_error.h".
 */


#include "ap_error.h"   // AP_ERROR, AP_ASSERT

#include <type_traits>  // std::is_pointer
#include <stdexcept>    // std::invalid_argument exception

#include <string>


#ifndef ADVANCED_PROGRAMMING_EXAM_2020_AP_ERROR_H
#define ADVANCED_PROGRAMMING_EXAM_2020_AP_ERROR_H

 /** Returns true it the template type is a pointer.*/
template<typename T>
inline bool is_pointer(const T&)
{
    return std::is_pointer<T>::value;
}


/**
 * \def AP_ERROR_ARGS(var_type, var)
 * This macro supports the macro AP_ASSERT_IF_NULLPTR, providing arguments for
 * the macro AP_ERROR(...) and AP_ASSERT(...).
 */
#define AP_ERROR_ARGS(var)                                                    \
    (!(is_pointer(var) && ! var )), std::invalid_argument /*check if it is not nullptr*/

 /**
  * \def AP_ASSERT_IF_NULLPTR(var_type, var, msg, is_error )
  * This macro checks if a variable is a nullptr, by using AP_ERROR or AP_ASSERT.
  * @param var      The variable.
  * @param var_name The name of the variable.
  * @param is_error Flag: if true, AP_ERROR is used, otherwise AP_ASSERT.
  */
#define AP_ERROR_IF_NULLPTR(var, var_name, is_error )                         \
    std::string msg {"The variable " #var_name " cannot be nullptr."};        \
    if(is_error)                                                              \
    {                                                                         \
        AP_ERROR(AP_ERROR_ARGS(var)) << msg << std::endl;                     \
    }                                                                         \
    else                                                                      \
    {                                                                         \
        AP_ASSERT(AP_ERROR_ARGS(var)) << msg << std::endl;                    \
    }


# endif // ADVANCED_PROGRAMMING_EXAM_2020_AP_ERROR_H