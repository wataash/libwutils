//===-- wutils.cc - wataash's single file library  --------------*- C++ -*-===//
//
// License: MIT
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Coding style follows "LLVM Coding Standards"
/// https://llvm.org/docs/CodingStandards.html
///
/// https://scrapbox.io/wataash/libwutils
///
//===----------------------------------------------------------------------===//

#ifndef WUTILS_CC
#define WUTILS_CC

#include <sys/cdefs.h>

//===----------------------------------------------------------------------===//
// decl: C++
//===----------------------------------------------------------------------===//

#ifdef __cplusplus

#include <cstddef>
#include <string>

namespace nswutils {

// C++: 0 string

std::string hex(const unsigned char *data, size_t len);

// C++: misc

// C++: z draft

} // namespace nswutils

#endif //__cplusplus

//===----------------------------------------------------------------------===//
// decl: C
//===----------------------------------------------------------------------===//

__BEGIN_DECLS

// BSD compat
// http://cvsweb.netbsd.org/bsdweb.cgi/src/share/misc/style?rev=HEAD&content-type=text/x-cvsweb-markup
#include <sys/param.h>
#include <sys/types.h>

#include <stdarg.h>
#include <stdbool.h>

void w_sandbox(void);

// C: 0 string

int w_hex(const unsigned char *bytes, size_t len_bytes, char *buf,
          size_t len_buf, bool x);
char *w_sprintf3(const char *format, ...)
    __attribute__((__format__(printf, 1, 2)));

// C: logging

// https://stackoverflow.com/a/36120843/4085441
void w_vl(const char *func, const char *format, va_list ap)
    __attribute__((__format__(__printf__, 2, 0)));
void w_l(const char *func, const char *format, ...)
    __attribute__((__format__(printf, 2, 3)));
void w_lw(const char *func, const char *format, ...)
    __attribute__((__format__(printf, 2, 3)));
void w_li(const char *func, const char *format, ...)
    __attribute__((__format__(printf, 2, 3)));
void w_ld(const char *func, const char *format, ...)
    __attribute__((__format__(printf, 2, 3)));

#define w_w(format, ...) w_lw(__func__, format, ##__VA_ARGS__)
#define w_i(format, ...) w_li(__func__, format, ##__VA_ARGS__)
#define w_d(format, ...) w_ld(__func__, format, ##__VA_ARGS__)

// C: misc

void w_loop_(void);
void w_loop10_(void);
#define w_loop()                                                               \
  do {                                                                         \
    w_d("loop");                                                               \
    w_loop_();                                                                 \
    w_d("loop return");                                                        \
  } while (0)
#define w_loop10()                                                             \
  do {                                                                         \
    w_d("loop10");                                                             \
    w_loop10_();                                                               \
    w_d("loop10 return");                                                      \
  } while (0)
void w_lsof(void);
void w_print_colors(void);
void w_stop(void);
void w_systemf(const char *format, ...)
    __attribute__((__format__(printf, 1, 2))); // should be restrict?

// C: z draft

unsigned long w_hash(const char *str);

__END_DECLS

#endif // WUTILS_CC

//===----------------------------------------------------------------------===//
// impl: include
//===----------------------------------------------------------------------===//

// string comparison is so hard... https://stackoverflow.com/q/2335888/4085441
// // __BASE_FILE__ for clang; TODO: gcc
// #if __BASE_FILE__ == ../libwutils/wutils.cc.compile.cc

#if defined(WUTILS_IMPL_IF_CXX) && defined(__cplusplus)
#pragma message "defining WUTILS_IMPL..."
#define WUTILS_IMPL
#endif // defined(WUTILS_IMPL_IF_CXX) && defined(__cplusplus)

#ifdef WUTILS_IMPL

#ifndef __cplusplus
#error C++ compiler is needed! (trying to compile with C compiler?)
#endif // __cplusplus

// c++
#include <csignal>
#include <cstdarg>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

// c
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//===----------------------------------------------------------------------===//
// impl: C++
//===----------------------------------------------------------------------===//

namespace nswutils {

// C++: 0 string

// https://stackoverflow.com/a/14051107/4085441
std::string hex(const unsigned char *data, size_t len) {
  std::stringstream ss;
  ss << std::hex;
  for (size_t i = 0; i < len; ++i) {
    ss << std::setw(2) << std::setfill('0') << (int)data[i];
  }
  return ss.str();
}

// C++: misc

class CWUtils {
public:
  int iii;

  explicit CWUtils() { iii = 0; }
  explicit CWUtils(int i) { iii = i; }
  ~CWUtils() { iii = -1; }

  void lsof() { _lsof(); }

protected:
  void foo();

private:
  void _lsof();
};

void CWUtils::_lsof() { std::system("true"); }

} // namespace nswutils

// C++: z draft

//===----------------------------------------------------------------------===//
// impl: C
//===----------------------------------------------------------------------===//

__BEGIN_DECLS
namespace {

void w_sandbox() {
  auto cp = new nswutils::CWUtils(); // size: 8
  auto c = nswutils::CWUtils(9);     // size: 4

  (void)cp->iii;
  (void)c.iii;

  c.lsof();

  delete (cp);
  cp = nullptr;
}

// C: 0 string

// XXX: __restrict__: g++ extension
// "static" is meaningless, just for annotation
static size_t w_strlcpy(char *__restrict__ dst, const char *__restrict__ src,
                        size_t dstsize) {
  if (dstsize == 0) {
    return 0;
  }
  (void)std::strncpy(dst, src, dstsize - 1);
  dst[dstsize - 1] = '\0';
  return std::strlen(dst);
}

int w_hex(const unsigned char *bytes, size_t len_bytes, char *buf,
          size_t len_buf, bool x) {
  auto h = nswutils::hex(bytes, len_bytes);
  if (x) {
    h = "0x" + h;
  }
  if (len_buf == 0) {
    return -1;
  }
  auto len = w_strlcpy(buf, h.c_str(), len_buf);
  if (len != h.length()) {
    return -1;
  }
  return 0;
}

char *w_sprintf3(const char *format, ...) {
  static char bufs[3][10240]; // 10KB * 3
  static unsigned int current_buf = 0;
  va_list ap;

  va_start(ap, format);
  int len = vsnprintf(bufs[current_buf], sizeof(bufs[0]), format, ap);
  (void)len; // TODO
  va_end(ap);

  char *ret = bufs[current_buf];
  current_buf = (current_buf + 1) % 3;
  return ret;
}

// C: logging

// TODO: pin_pid()

// TODO: API to select output stream

/// w_vl(__func__, "%s", ap)
/// => >>> XXXXX __func__ ap
///        ^^^^^ PID
void w_vl(const char *func, const char *format, va_list ap) {
  std::cerr << ">>> " << getpid() << ' ' << func << ' ';
  std::vfprintf(stderr, format, ap);
  std::cerr << std::endl;
}

/// w_l(__func__, "%s", "foo")
/// => XXXXX __func__ foo
void w_l(const char *func, const char *format, ...) {
  va_list ap;
  va_start(ap, format);

  w_vl(func, format, ap);

  va_end(ap);
}

// TODO: API to select output stream

// https://stackoverflow.com/a/36120843/4085441
__attribute__((__format__(__printf__, 2, 0))) static void
log_color(const char *func, const char *format, va_list ap, const char *color) {
  std::cerr << ">>> " << color << getpid() << ' ' << func << ' ';
  std::vfprintf(stderr, format, ap);
  std::cerr << "\x1b[0m" << std::endl;
}

void w_lw(const char *func, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  ::log_color(func, format, ap, "\x1b[33m");
  va_end(ap);
}

void w_li(const char *func, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  ::log_color(func, format, ap, "\x1b[36m");
  va_end(ap);
}

void w_ld(const char *func, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  ::log_color(func, format, ap, "\x1b[37m");
  va_end(ap);
}

// C: misc

void w_loop_(void) {
  static volatile int not_loop = 0; // set me to 1 with gdb
  if (not_loop)
    return;

  volatile long break_ = 1;
  while (break_ != 0) {
    // to quit: wait ~1s or `break_ = 0` with gdb
    break_++;
    // if (break_ == (long)1e9) { // tune me
    if (break_ == 5e8) {
      break;
    }
  }
}

void w_loop10_(void) {
  volatile long break_ = 1;
  while (break_ != 0) {
    break_++;
    if (break_ == 1e10) {
      break;
    }
  }
}

void w_lsof() {
  // exclude:
  // COMMAND     PID USER   FD   TYPE DEVICE SIZE/OFF     NODE NAME
  // lib_libev 31937  wsh  mem    REG  253,1  1365096 36315339
  // /usr/lib/x86_64-linux-gnu/libm-2.29.so
  w_systemf("lsof -p %jd | ag -v /usr/lib/x86_64",
            static_cast<std::intmax_t>(getpid()));
}

void w_print_colors() {
  std::cerr << "\x1b[30m black   \x1b[0m" << std::endl;
  std::cerr << "\x1b[31m red     \x1b[0m" << std::endl;
  std::cerr << "\x1b[32m green   \x1b[0m" << std::endl;
  std::cerr << "\x1b[33m yellow  \x1b[0m" << std::endl;
  std::cerr << "\x1b[34m blue    \x1b[0m" << std::endl;
  std::cerr << "\x1b[35m magenta \x1b[0m" << std::endl;
  std::cerr << "\x1b[36m cyan    \x1b[0m" << std::endl;
  std::cerr << "\x1b[37m white   \x1b[0m" << std::endl;
}

void w_stop() {
  // TODO: pthread_kill?
  w_w("SIGSTOP");
  kill(getpid(), SIGSTOP);
  w_w("SIGSTOP done");
}

void w_systemf(const char *format, ...) {
  char buf[100];
  va_list ap;

  va_start(ap, format);
  int len = std::vsnprintf(buf, sizeof(buf), format, ap);
  (void)len; // XXX: len vs sizeof(buf) not checked
  va_end(ap);

  std::fprintf(stderr, "\x1b[36m system %s \x1b[37m\n", buf);

  int err_ = std::system(buf);
  if (err_ != 0) {
    perror("system");
  }

  std::fprintf(stderr, "\x1b[0m");
}

// C: z draft

// https://stackoverflow.com/a/7666577/4085441
unsigned long w_hash(const char *str) {
  unsigned long hash = 5381;
  // "a" -> 177670
  // "b" -> 177671
  // "c" -> 177672

  // static unsigned long hash = 5381;

  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

} // namespace
__END_DECLS

#endif // WUTILS_IMPL
