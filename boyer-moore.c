#include "boyer-moore.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

static bool isMajority(int array[], int size, int candidate) {
    int majority = (size / 2) + 1;    
    int count = 0;
    for (int i = 0; i < size; ++i) {
        if (array[i] == candidate) {
            count ++;
        }

        if (count >= majority) { // Return as soon as we hit majority
            return true;
        } 
    }
    return false;
}

static int firstIndexOf(int array[], int size, int candidate) {
    for (int i = 0; i < size; ++i) {
        if (array[i] == candidate) {
            return i;
        }
    }

    assert(false && "Candidate must be in array when function is called");
}

int bm_majority(int array[], int size) {
    int candidate = 0;
    int count = 0;
    for (int i = 0; i < size; ++i) {
        if (count == 0) {
            candidate = array[i];
        }

        if (candidate == array[i]) {
            count ++;
        } else {
            count --;
        }
    }

    if (isMajority(array, size, candidate)) {
        return firstIndexOf(array, size, candidate);
    } else {
        return -1;
    }
}
