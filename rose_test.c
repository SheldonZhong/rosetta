#include "rose.h"

  static void
test_rose(struct rose* rose, u64 low, u64 high)
{
  bool exist = rose_range(rose, low, high);
  printf("low: %ld hight: %ld ", low, high);
  printf("%s\n", exist ? "exist" : "not exist");
}

  int
main(int argc, char** argv)
{
  (void)argc;
  (void)argv;
  // u64 keys[] = {2019, 10, 2, 2020, 486, 10000, 2135, 987321, 123};
  u64 keys[] = {2, 3, 13, 19, 23, 29, 31, 37, 123, 202};
  // u64 keys[] = {123};
  const u64 num = 10;
  struct rose* rose = rose_create(num);
  printf("insert: ");
  for (u64 i = 0; i < num; i++) {
    printf("%ld ", keys[i]);
    rose_insert(rose, keys[i]);
  }
  printf("\n");
  test_rose(rose, 20, 30);
  test_rose(rose, 40, 73);
  test_rose(rose, 100, 130);
  test_rose(rose, 140, 201);
  test_rose(rose, 210, 220);
  rose_destroy(rose);
}

