#include "psort.h"
#include <omp.h>
#include <iostream>
#include <cmath>
#define ull long long
#define INF 1e32
// merge sort implementation
void merge(uint32_t *a, ull low, ull mid, ull high)
{
    uint32_t *b = new uint32_t[high - low + 1];
    ull k = 0;
    ull i = low, j = mid + 1;
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
    ull p = low;
    for (ull l = 0; l < k; l++)
    {
        a[p++] = b[l];
    }
}
void swap(uint32_t *a, uint32_t *b)
{
    uint32_t t = *a;
    *a = *b;
    *b = t;
}
ull partition_helper(uint32_t *arr, ull low, ull high)
{
    // courtesy : gfg standard algo implementations
    ull pivot = arr[high];
    ull i = (low - 1);
    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(uint32_t arr[], ull low, ull high)
{
    // courtesy : gfg standard algo implementations
    if (low < high)
    {
        ull pi = partition_helper(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void InsertionSort(uint32_t *arr, ull low, ull high)
{
    // courtesy : gfg standard algo implementations
    ull i, key, j;
    for (ull i = low + 1; i <= high; i++)
    {
        key = arr[i];
        j = i - 1;
        bool flag = false;
        while (j >= low && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            if (j == 0)
            {
                flag = true;
                break;
            }
            else
            {
                j = j - 1;
            }
        }
        if (flag)
        {
            arr[0] = key;
        }
        else
        {
            arr[j + 1] = key;
        }
    }
}

void mergesort(uint32_t *a, ull low, ull high)
{
    if (high - low + 1 < 32)
    {
        InsertionSort(a, low, high);
        return;
    }
    if (high <= low)
    { return; }
    ull mid;
    mid = (low + (high - low) / 2);
    mergesort(a, low, mid);
    mergesort(a, mid + 1, high);
    merge(a, low, mid, high);
}
void StandardSort(uint32_t *data, ull n)
{
    // mergesort(data, 0, n - 1);
    quickSort(data, 0, n - 1);
    // InsertionSort(data, 0, n - 1);
}
ull partition(uint32_t *arr, ull low, ull high, ull pivot)
{
    if (low >= high)
    { return high; }
    while (low < high)
    {
        if (arr[low] <= pivot)
        { low++; }
        else
        {
            // uint32_t temp = arr[low];
            // arr[low] = arr[high];
            // arr[high] = temp;
            // high--;
            std::swap(arr[low], arr[high--]);
        }
    }
    if (arr[high] > pivot) // case when we get last element
    { return high - 1; }
    return high;
}
uint32_t *split(uint32_t *data, uint32_t *splitting_values, ull n, ull p)
{
    uint32_t *pointers = new uint32_t[p];
    for (ull i = 0; i < p; i++)
    {
        pointers[i] = 0;
        ull being, end = n;
        if (i == 0)
        { being = 0; }
        else
        { being = pointers[i - 1]; }
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
void split_improved(uint32_t *data, uint32_t *splitting_values, ull dmin, ull dmax, ull smin, ull smax, ull *pointers)
{
    // std::cout << "splitting on dmin: " << dmin << " dmax: " << dmax << " smin: " << smin << " smax: " << smax << std::endl;
    if (dmax <= dmin || smax < smin)
    {
        return;
    }
    ull smid = smin + (smax - smin) / 2;
    ull split_index = partition(data, dmin, dmax, splitting_values[smid]);
    // std::cout<<"split_index: "<<split_index<<std::endl;
    pointers[smid] = split_index;
    #pragma omp task shared(data, splitting_values, pointers)
    split_improved(data, splitting_values, dmin, split_index, smin, smid - 1, pointers);
    // std::cout << "Left done" << std::endl;
    #pragma omp task shared(data, splitting_values, pointers)
    split_improved(data, splitting_values, split_index + 1, dmax, smid + 1, smax, pointers);
    // std::cout << "Right done" << std::endl;
    #pragma omp taskwait
}
void ParallelSort(uint32_t *A, uint32_t n, int p)
{
    if (n <= p * p || p <= 1)
    {
        StandardSort(A, n);
        return;
    }
    // std::cout<<"n: "<<n<<" p: "<<p<<std::endl;
    uint32_t bucket_size = ceil(n / p) + 1;
    uint32_t *bucketStart = new uint32_t[p];
    bucketStart[0] = 0;
    for (ull i = 1; i < n % p + 1; i++)
    {
        bucketStart[i] = bucketStart[i - 1] + bucket_size;
    }
    for (ull i = n % p + 1; i < p; i++)
    {
        bucketStart[i] = bucketStart[i - 1] + bucket_size - 1;
    }
    uint32_t *R = new uint32_t[p * p];
    for (ull i = 0; i < p; i++)
    {
        for (ull j = 0; j < p; j++)
        {
            R[i * p + j] = A[bucketStart[i] + j];
        }
    }
    StandardSort(R, p * p);
    // std::cout<<"Standard sort done"<<std::endl;
    uint32_t *S = new uint32_t[p - 1];
    for (ull i = 0; i < p - 1; i++)
    {
        S[i] = R[(i + 1) * p];
    }
    ull *pointers = new ull[p - 1];
    split_improved(A, S, 0, n - 1, 0, p - 2, pointers);
    // std::cout<<"splits done"<<std::endl;
    uint32_t threshold = 2 * n / p;
    for (ull i = 0; i < p; i++)
    {
        //     std::cout<<"i in for: "<<i<<std::endl;
        {
            ull size, start;
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
// seqcalls++;
// std::cout << "parallel sort" << std::endl;
// std::cout << "std sort" << std::endl;
// std::cout << "start: " << start << " size: " << size << std::endl;
// StandardSort(B[i].data(), B[i].size());
                #pragma omp task shared(A)
                {   // std::sort(A+start,A+start+size);
                    StandardSort(A + start, size);
                }
            }
            else
            {
                // parcalls++;
                // std::cout << "parallel sort" << std::endl;
                // std::cout << "start: " << start << " size: " << size << std::endl;
                // create a task
                #pragma omp task shared(A)
                ParallelSort(A + start, size, p);
                // ParallelSort(A + start, size, p);
            }
        }
    }
    #pragma omp taskwait
}