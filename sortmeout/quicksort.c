// quickSort.c

#include <quicksort.h>


//**********************//
// FUNCTION DEFINITIONS //
//**********************************************************************


void quickSort (int *data, int l, int r) {
  int j;
  
  if (l < r) {
    j = partition (data, l, r);        // divide and conquer
    quickSort (data, l, j - 1);
    quickSort (data, j + 1, r);
  }
}


int partition (int *data, int l, int r) {
  int pivot, i, j, t;
  
  pivot = data[l];
  i = l;
  j = r + 1;
		
  while (1) {
    do ++i; while (data[i] <= pivot && i <= r);
    do --j; while (data[j] > pivot);
    
    if (i >= j) break;

    t = data[i];
    data[i] = data[j];
    data[j] = t;
  }

  t = data[l];
  data[l] = data[j];
  data[j] = t;

  return j;
}
