#include <tuple>

#include <gtest/gtest.h>

#include "wutils.cc"

namespace {

TEST(CxxTest, 0String) {
  auto tmp = "0x" + nswutils::hex((const unsigned char *)"hello", 6);
  ASSERT_EQ(tmp, "0x68656c6c6f00");
  asm("nop");
}

TEST(CTest, 0String) {
  // 0x68656c6c6f00 NUL
  // 12345678901234 5
  char buf12[12];
  char buf13[13];
  char buf14[14];
  char buf15[15];

  ASSERT_EQ(w_hex((unsigned char *)"hello", sizeof("hello"), buf12,
                  sizeof(buf12), false),
            -1);
  ASSERT_EQ(w_hex((unsigned char *)"hello", sizeof("hello"), buf13,
                  sizeof(buf13), false),
            0);
  ASSERT_EQ(w_hex((unsigned char *)"hello", sizeof("hello"), buf14,
                  sizeof(buf14), true),
            -1);
  ASSERT_EQ(w_hex((unsigned char *)"hello", sizeof("hello"), buf15,
                  sizeof(buf15), true),
            0);
  ASSERT_STREQ(buf12, "68656c6c6f0");
  ASSERT_STREQ(buf13, "68656c6c6f00");
  ASSERT_STREQ(buf14, "0x68656c6c6f0");
  ASSERT_STREQ(buf15, "0x68656c6c6f00");
}

TEST(CTest, Logger) {
  std::setbuf(stdout, nullptr);
  // TODO: assertion
  // testing::internal::CaptureStdout();
  w_l(__func__, "foo %d", 9);
  w_lw(__func__, "foo %d", 9);
  w_li(__func__, "bar");
  w_ld(__func__, "%s", "baz");

  w_w("foo %d", 9);
  w_i("bar");
  w_d("%s", "baz");

  // auto tmp = testing::internal::GetCapturedStdout();
}

TEST(Suite0, sandbox) {
  w_sandbox();
  // w_loop();
  // w_loop10();
  asm("nop");
}

} // namespace
