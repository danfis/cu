#include <stdio.h>
#include "cu.h"

TEST(testFunction)
{
    printf("Ahoj\n");
    assertTrue(0);
}

TEST(testFunction2)
{
    assertEquals(1, 1);
    assertNotEquals(1, 0);
}

TEST(testFunction3)
{
    assertEquals(0, 1);
    assertNotEquals(1, 0);
}

TEST_SUITE(testSuiteName)
{
    TEST_ADD(testFunction),
    TEST_ADD(testFunction2),
    TEST_ADD(testFunction3),
    TEST_SUITE_CLOSURE
};

TEST_SUITE(testSuiteName2)
{
    TEST_ADD(testFunction2),
    TEST_SUITE_CLOSURE
};

int main(int argc, char *argv[])
{
    RUN(testSuiteName);
    RUN(testSuiteName2);
    PRINT_RESULTS;

    return 0;
}
