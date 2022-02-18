#include "psort.h"
#include <omp.h>
#include <iostream>
#include <cmath>

#define INF 1e32
int counts = 0;
// bool debug = true;
bool debug = false;
uint32_t seqcalls = 0, parcalls = 0;



// merge sort implementation
void merge(uint32_t *a, int low, int mid, int high)
{
    uint32_t *b = new uint32_t[high - low + 1];
    int k = 0;
    int i = low, j = mid + 1;

    while (i <= mid && j <= high)
    {
        if (a[i] <= a[j])
        {
            b[k++] = a[i];
            i++;
        }
        else
        {
            b[k++] = a[j];
            j++;
        }
    }
    while (i <= mid)
    {
        b[k++] = a[i++];
    }
    while (j <= high)
    {
        b[k++] = a[j++];
    }
    int p = low;
    for (int l = 0; l < k; l++)
    {
        a[p++] = b[l];
    }
}
void InsertionSort(uint32_t *arr, int low, int high)
{
    // insertion sort implementation
    int i, key, j;
    for (uint32_t i = low + 1; i <= high; i++)
    {
        key = arr[i];
        j = i - 1;
        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= low && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void mergesort(uint32_t *a, int low, int high)
{
    if (high - low + 1 < 32)
    {
        InsertionSort(a, low, high);
        return;
    }
    if (high <= low)
        return;
    int mid;
    mid = (low + (high - low) / 2);

    mergesort(a, low, mid);
    mergesort(a, mid + 1, high);
    merge(a, low, mid, high);
}
void StandardSort(uint32_t *data, uint32_t n)
{
    // implement merge sort
    mergesort(data, 0, n - 1);
    // InsertionSort(data, 0, n - 1);
}
int partition(uint32_t *arr, int low, int high, uint pivot)
{
    if (low >= high)
        return high;
    // if (low >= high)
    //     return high;
    // int i = (low - 1); // Index of smaller element

    // for (int j = low; j <= high - 1; j++)
    // {
    //     // If current element is smaller than or
    //     // equal to pivot
    //     if (arr[j] <= pivot)
    //     {
    //         i++; // increment index of smaller element
    //         std::swap(arr[i], arr[j]);
    //     }
    // }
    // std::swap(arr[i + 1], arr[high]);
    // return (i + 1);

    while (low != high)
    {
        if (arr[low] <= pivot)
            low++;
        else
        {
            uint32_t temp = arr[low];
            arr[low] = arr[high];
            arr[high] = temp;
            high--;
        }
    }
    if (arr[high] > pivot)
        return high - 1;
    return high;
}
uint32_t *split(uint32_t *data, uint32_t *splitting_values, uint32_t n, uint32_t p)
{
    uint32_t *pointers = new uint32_t[p];
    for (uint32_t i = 0; i < p; i++)
    {

        pointers[i] = 0;
        uint32_t being, end = n;
        if (i == 0)
            being = 0;
        else
            being = pointers[i - 1];
        while (being < end)
        {
            if (data[being] <= splitting_values[i])
            {
                being++;
            }
            else
            {
                end--;
                std::swap(data[being], data[end]);
            }
        }
        pointers[i] = end;
    }
    return pointers;
}
// int partition(uint32_t *data, int low, int high, uint32_t pivot_value)
// {
//     // all values <= pivot_value are on the left
//     // all values > pivot_value are on the right

//     while (low < high)
//     {
//         while (low < high && data[high] > pivot_value)
//             high--;
//         data[low] = data[high];
//         while (low < high && data[low] <= pivot_value)
//             low++;
//         data[high] = data[low];
//     }
//     data[low] = pivot_value;
//     return low;
// }
void split_improved(uint32_t *data, uint32_t *splitting_values, int dmin, int dmax, int smin, int smax, uint32_t *pointers)
{
    if (debug)
    {
        std::cout << "splitting on dmin: " << dmin << " dmax: " << dmax << " smin: " << smin << " smax: " << smax << " val: " << splitting_values[smin + (smax - smin) / 2] << std::endl;
        // print data
        for (uint32_t i = dmin; i < dmax + 1; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
    if (dmax <= dmin || smax < smin)
    {
        return;
    }
    int smid = smin + (smax - smin) / 2;
    int split_index = partition(data, dmin, dmax, splitting_values[smid]);
    if (debug)
    {
        for (uint32_t i = dmin; i < dmax + 1; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "index: " << split_index << std::endl;
    }
    pointers[smid] = split_index;
#pragma omp task shared(data, splitting_values)
    split_improved(data, splitting_values, dmin, split_index, smin, smid - 1, pointers);
    // std::cout << "Left done" << std::endl;
#pragma omp task shared(data, splitting_values)
    split_improved(data, splitting_values, split_index + 1, dmax, smid + 1, smax, pointers);
#pragma omp taskwait
}
void ParallelSort(uint32_t *A, uint32_t n, int p)
{
    counts++;
    if (n <= p * p || p <= 1)
    {
        StandardSort(A, n);
        return;
    }
    uint32_t bucket_size = ceil(n / p) + 1;
    uint32_t *bucketStart = new uint32_t[p];
    bucketStart[0] = 0;
    for (int i = 1; i < n % p + 1; i++)
    {
        bucketStart[i] = bucketStart[i - 1] + bucket_size;
    }
    for (int i = n % p + 1; i < p; i++)
    {
        bucketStart[i] = bucketStart[i - 1] + bucket_size - 1;
    }
    uint32_t *R = new uint32_t[p * p];
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < p; j++)
        {
            R[i * p + j] = A[bucketStart[i] + j];
        }
    }
    StandardSort(R, p * p);
    uint32_t *S = new uint32_t[p - 1];
    for (int i = 0; i < p - 1; i++)
    {
        S[i] = R[(i + 1) * p];
    }

    uint32_t *pointers = new uint32_t[p - 1];
    split_improved(A, S, 0, n - 1, 0, p - 2, pointers);
    if (debug)
    {
        for (int i = 0; i < p - 1; i++)
        {
            std::cout << S[i] << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < n; i++)
        {
            std::cout << A[i] << " ";
        }
        std::cout << std::endl;

        for (int i = 0; i < p - 1; i++)
        {
            std::cout << "pointers[" << i << "]: " << pointers[i] << std::endl;
        }
        std::cout << std::endl;
    }
    uint32_t threshold = n / p;
    // if (counts > 30)
    //     pointers[1000000000000] = 0;
    // std::cout << "threshold: " << threshold << std::endl;
    for (int i = 0; i < p; i++)
    {
#pragma omp task shared(A)
        {
            uint32_t size, start;
            if (i == 0)
            {
                size = pointers[i] + 1;
                start = 0;
            }
            else if (i == p - 1)
            {
                size = n - pointers[i - 1];
                start = pointers[i - 1];
            }
            else
            {
                size = pointers[i] - pointers[i - 1];
                start = pointers[i - 1] + 1;
            }
            // std::cout << "size: " << size << std::endl;
            if (size == 0)
            {
            }

            else if (size < threshold || n / p < p)
            {
                seqcalls++;
                // std::cout << "parallel sort" << std::endl;
                // std::cout << "std sort" << std::endl;
                // std::cout << "start: " << start << " size: " << size << std::endl;
                // StandardSort(B[i].data(), B[i].size());
                StandardSort(A + start, size);
            }
            else
            {
                parcalls++;
                // std::cout << "parallel sort" << std::endl;
                // std::cout << "start: " << start << " size: " << size << std::endl;
                // create a task

                ParallelSort(A + start, size, p);
                // ParallelSort(A + start, size, p);
            }
        }
    }
#pragma omp taskwait
}