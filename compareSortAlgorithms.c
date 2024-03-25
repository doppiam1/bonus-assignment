#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r)
	{
		int mid = l + (r - l) / 2;

		mergeSort(pData, l, mid);
		mergeSort(pData, mid + 1, r);

		int i, j, k;
		int firstHalf = mid - l + 1;
		int secondHalf = r - mid;

		//int leftArr[firstHalf];
		//int rightArr[secondHalf];

		int* leftArr = Alloc(firstHalf * sizeof(int));
		int* rightArr = Alloc(secondHalf * sizeof(int));

		for (int i = 0; i < firstHalf; i++)
		{
			leftArr[i] = pData[l + i];
		}
		for (int j = 0; j < secondHalf; j++)
		{
			rightArr[j] = pData[j + mid + 1];
		}

		i = 0;
		j = 0;
		k = l;

		while (i < firstHalf && j < secondHalf)
		{
			if (leftArr[i] <= rightArr[j])
			{
				pData[k] = leftArr[i];
				i++;
			}
			else
			{
				pData[k] = rightArr[j];
				j++;
			}
			k++;
		}

		while (i < firstHalf)
		{
			pData[k] = leftArr[i];
			i++;
			k++;
		}

		while (j < secondHalf)
		{
			pData[k] = rightArr[j];
			j++;
			k++;
		}	

		DeAlloc(leftArr);
		DeAlloc(rightArr);
	}
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* arr, int n)
{
	int i, j, k;

	for (i = 1; i < n; i++)
	{
		k = arr[i];
		j = i - 1;
	}
	while (j >= 0 && arr[j] > k) 
	{
        arr[j + 1] = arr[j];
        j = j - 1;
    }
        arr[j + 1] = k;
}


// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* arr, int n)
{
  int i, j, totalSwaps = 0;
  int swapCounts[n];
  for (int i = 0; i < n; i++) {
    swapCounts[i] = 0;
  }

  for (int i = 0; i < n - 1; i++) {
    int swapped = 0;
    for (int j = 0; j < n - i - 1; j++) {
      if ((arr[j] > arr[j + 1])) {
        swap(&arr[j], &arr[j + 1]);
        swap(&swapCounts[j], &swapCounts[j + 1]);
        swapCounts[j]++;
        swapCounts[j + 1]++;
        swapped = 1;
      }
    }
    if (swapped == 0) {
      break;
    }
  }

  for (int i = 0; i < n; i++) {
    totalSwaps += swapCounts[i];
  }
  totalSwaps = totalSwaps / 2;

  return arr;
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* arr, int n)
{
	int i, j, minIndex;
  	int totalSwaps = 0;
  	int swapCounts[n];

  	for (int i = 0; i < n; i++) 
	{
    	swapCounts[i] = 0;
  	}

  for (i = 0; i < n - 1; i++) 
  {
    	minIndex = i;
    	for (int j = i + 1; j < n; j++) 
		{
      		if (arr[j] < arr[minIndex]) 
			{
        		minIndex = j;
      		}
		}

		if (minIndex != i) 
		{
    		swap(&arr[minIndex], &arr[i]);
      		swap(&swapCounts[minIndex], &swapCounts[i]);
      		swapCounts[minIndex]++;
      		swapCounts[i]++;
      		totalSwaps++;
    	}
	}
	return arr;
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
}
