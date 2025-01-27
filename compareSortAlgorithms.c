#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void printArray(int[], int);

void swap(int* array, int index1, int index2)
{
    int temp = array[index1];
    array[index1] = array[index2];
    array[index2] = temp;
}

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

void heapify(int arr[], int root, int n)
{
	int largest = root;
	int left = 2 * root + 1;
	int right = 2 * root + 2;

	if (left < n && arr[largest] < arr[left])
	{
		largest = left;
	}
	if (right < n && arr[largest] < arr[right])
	{
		largest = right;
	}

	if (largest != root)
	{
		swap(arr, largest, root);
		heapify(arr, largest, n);
	}
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	if (n <= 0)
	{
		return;
	}

	//Heapify
	for (int i = n/2 - 1; i >= 0; i--)
	{
		heapify(arr, i, n);
	}

	for (int i = n; i > 1; i--)
	{
		//Remove last element by swapping it to the back
		swap(arr, 0, i-1);

		//Re-heapify after removing the top element
		heapify(arr, 0, i-1);
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	//Terminating condition
	if (l >= r)
	{
		return;
	}

	//Divide the array in half
	int mid = (l + r) / 2;
	int lSize = mid + 1;
	int rSize = r - mid;

	//Merge sort each subsection
	mergeSort(pData, l, mid);
	mergeSort(pData, mid + 1, r);

	//Merge the portions back together
	int* temp = (int*)Alloc((r + 1 - l) * sizeof(int));
	int lIndex = l;
	int rIndex = mid + 1;
	int curr = 0;

	while(lIndex <= mid && rIndex <= r)
	{
		if (pData[lIndex] <= pData[rIndex])
		{
			temp[curr] = pData[lIndex];
			lIndex++;
			curr++;
		}
		else
		{
			temp[curr] = pData[rIndex];
			rIndex++;
			curr++;
		}
	}

	while(lIndex <= mid)
	{
		temp[curr] = pData[lIndex];
		lIndex++;
		curr++;
	}

	while(rIndex <= r)
	{
		temp[curr] = pData[rIndex];
		rIndex++;
		curr++;
	}

	for (int i = 0; i < r + 1 - l; i++)
	{
		pData[i + l] = temp[i];
	}

	DeAlloc(temp);
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i; j >= 0; j--)
		{
			if (pData[j] < pData[j+1])
			{
				break;
			}
			swap(pData, j, j+1);
		}
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j < n - i; j++)
        {
            if (pData[j] > pData[j+1])
            {
                swap(pData, j, j+1);
            }
        }
    }
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
    for (int i = 0; i < n-1; i++)
    {
        int min = i;
        for (int j = i; j < n; j++)
        {
            if (pData[j] < pData[min])
            {
                min = j;
            }
        }

        if (min == i)
        {
            continue;
        }

        swap(pData, i, min);
    }
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
		for (int i = 0; i < dataSz; i++)
		{
			fscanf(inFile, "%d", &((*ppData)[i]));
		}
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100, tempMax = 100;
	printf("\tData:\n\t");
	if (dataSz < 100)
	{
		sz = dataSz;
		tempMax = dataSz;
	}
	for (i=0;i<tempMax;++i)
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
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
