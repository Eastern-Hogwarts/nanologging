/*!
 * \copyright Copyright (c) 2021
 * \file nanologging.h
 * \brief defines logging macros of nanologging
 */

#ifndef NANOLOGGING_H_
#define NANOLOGGING_H_

#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace nanologging {

class DateLogger {
public:
  DateLogger() {
#if defined(_MSC_VER)
    // set time env var for MSVS
    // https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/tzset?view=msvc-160
    _tzset();
#endif
  }

  const char *HumanDate() {
#ifdef _MSC_VER
    _strtime_s(m_buffer, sizeof(m_buffer));
#else
    time_t time_value = time(nullptr);
    struct tm *pnow;
#ifndef _WIN32
    struct tm now;
    pnow = localtime_r(&time_value, &now);
#else
    pnow = localtime(&time_value); // NOLINT(*)
#endif // _WIN32
    snprintf(m_buffer, sizeof(m_buffer), "%02d:%02d:%02d", pnow->tm_hour,
             pnow->tm_min, pnow->tm_sec);
#endif // _MSC_VER
    return m_buffer;
  }

  static DateLogger &Get() {
    thread_local DateLogger obj_;
    return obj_;
  }

private:
  char m_buffer[9];
};

template <typename... Messages> std::string PrintMessage(Messages &&...msgs) {
  std::ostringstream sm;
  (sm << ... << msgs) << std::endl;
  return sm.str();
}

} // namespace nanologging

#define MESSAGE_AND_THROW_(...)                                                \
  {                                                                            \
    auto msg = nanologging::PrintMessage(__VA_ARGS__);                         \
    std::cerr << "[LOG ERROR] - "                                              \
              << nanologging::DateLogger::Get().HumanDate()                    \
              << " - File: " << __FILE__ << ":" << __LINE__ << ": " << msg;    \
    throw std::runtime_error(msg);                                             \
  }

#define LOG_(level, ...)                                                       \
  {                                                                            \
    std::cerr << "[LOG " #level << "] - "                                      \
              << nanologging::DateLogger::Get().HumanDate()                    \
              << " - File: " << __FILE__ << ":" << __LINE__ << ": "            \
              << nanologging::PrintMessage(__VA_ARGS__);                       \
  }

#define LOG_INFO(...) LOG_(INFO, __VA_ARGS__)
#define LOG_WARN(...) LOG_(WARN, __VA_ARGS__)
#define LOG_ERROR(...) MESSAGE_AND_THROW_(__VA_ARGS__)

#define CHECK(x)                                                               \
  if (!(x)) {                                                                  \
    LOG_ERROR("Check failed: " #x)                                             \
  }

#define CHECK_EQ(x, y) CHECK(x == y)
#define CHECK_NE(x, y) CHECK(x != y)
#define CHECK_LT(x, y) CHECK(x < y)
#define CHECK_LE(x, y) CHECK(x <= y)
#define CHECK_GT(x, y) CHECK(x > y)
#define CHECK_GE(x, y) CHECK(x >= y)

#endif // NANOLOGGING_H_
