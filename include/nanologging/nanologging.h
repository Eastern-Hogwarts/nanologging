/*!
 * @copyright Copyright (c) 2021
 * @file nanologging.h
 * @brief defines logging macros of nanologging
 */

#ifndef NANOLOGGING_H_
#define NANOLOGGING_H_

#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>

// force-inline macro
#if defined(__GNUC__) || defined(__clang__)
#define NANOLOGGING_ALWAYS_INLINE inline __attribute__((__always_inline__))
#elif defined(_MSC_VER)
#define NANOLOGGING_ALWAYS_INLINE __forceinline
#else
#define NANOLOGGING_ALWAYS_INLINE inline
#endif

namespace nanologging {

/**
 * @brief exception class that will be thrown by
 *  default logger
 */
struct Error : public std::runtime_error {
  /**
   * @brief Construct a new Error object
   * @param s the error msg
   */
  explicit Error(const std::string &s) : std::runtime_error(s) {}
};

/** @brief Time logger */
class DateLogger {
public:
  DateLogger() {
#if defined(_MSC_VER)
    // set time env var for MSVS
    // https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/tzset?view=msvc-160
    _tzset();
#endif
  }

  const char* HumanDate() {
#ifdef _MSC_VER
    _strtime_s(m_buffer, sizeof(m_buffer));
#else
    time_t time_value = time(nullptr);
    struct tm *pnow;
#ifndef _WIN32
    struct tm now;
    pnow = localtime_r(&time_value, &now);
#else
    pnow = localtime(&time_value);  // NOLINT(*)
#endif // _WIN32
    snprintf(m_buffer, sizeof(m_buffer), "%02d:%02d:%02d",
             pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
#endif // _MSC_VER
    return m_buffer;
  }

private:
  char m_buffer[9];
};



} // namespace nanologging


#endif // NANOLOGGING_H_
