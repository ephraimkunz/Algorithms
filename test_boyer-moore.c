#include "boyer-moore.h"
#include <assert.h>
#include <stdio.h>

void runTests() {
    // Fail test (not a majority)
    int input[] = {1,3,5,7,3,7,8,8,9,8};
    int size = sizeof(input) / sizeof(input[1]);

    int result = bm_majority(input, size);
    assert(result == -1);

    // Pass test (majority exists)
    int input2[] = {1,3,5,7,3,3,7,8,8,9,3,8,3,3,3,3,3};
    size = sizeof(input2) / sizeof(input2[1]);

    result = bm_majority(input2, size);
    assert(result == 1);
}

int main() {
    runTests();
}