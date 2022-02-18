#include "classify_v1.h"
#include <omp.h>
#include <atomic>   
#define STRIDE 1
std::atomic <int> freq[1001]; 
Data classify(Data &D, const Ranges &R, unsigned int numt)
{ // Classify each item in D into intervals (given by R). Finally, produce in D2 data sorted by interval
    assert(numt < MAXTHREADS);
    int RLen = R.num();
    int nData = D.ndata;
    Counter counts[RLen]; // I need on counter per interval. Each counter can keep pre-thread subcount.
#pragma omp parallel num_threads(numt)
    {
        int tid = omp_get_thread_num(); // I am thread number tid
        for (int i = tid; i < nData; i += numt)
        {                                                     // Threads together share-loop through all of Data
            D.data[i].value = R.range(D.data[i].key); // For each data, find the interval of data's key,
                                                              // and store the interval id in value. D is changed.
            // counts[v].increase(tid);                          // Found one key in interval v
        }
    }
#pragma omp parallel num_threads(numt)
    {
        int tid = omp_get_thread_num(); // I am thread number tid
        for (int i = tid; i < nData; i += numt)
        {                                                     // Threads together share-loop through all of Data
            int v = D.data[i].value; // For each data, find the interval of data's key,
                                                              // and store the interval id in value. D is changed.
            counts[v].increase(tid);                          // Found one key in interval v
        }
    }

    // Accumulate all sub-counts (in each interval;'s counter) into rangecount
    unsigned int *rangecount = new unsigned int[RLen];
    //    memset(rangecount,0,sizeof(rangecount));
    for (int r = 0; r < RLen; r++)
    { // For all intervals
        rangecount[r] = 0;
        freq[r]=0;
        for (int t = 0; t < numt; t++) // For all threads
            rangecount[r] += counts[r].get(t);
        // std::cout << rangecount[r] << " elements in Range " << r << "\n"; // Debugging statement
    }

    // Compute prefx sum on rangecount.
    for (int i = 1; i < RLen; i++)
    {
        rangecount[i] += rangecount[i - 1];
    }

    // Now rangecount[i] has the number of elements in intervals before the ith interval.

    Data D2 = Data(nData); // Make a copy

// #pragma omp parallel num_threads(numt)
//     {
//         int tid = omp_get_thread_num();
//         for (int r = tid; r < RLen; r += numt)
//         { // Thread together share-loop through the intervals
//           //  int rcount = 0;
//             int rcount = 0;
//             // int rminusone = r - 1;

//             for (int d = 0; d < nData; d++) // For each interval, thread loops through all of data and
//                 if (D.data[d].value == r)   // If the data item is in this interval

//                     // rcount++;
//                     D2.data[rangecount[r - 1] + rcount++] = D.data[d]; // Copy it to the appropriate place in D2.
//                                                                        //    D2.data[rangecount[r - 1] + rcount++] = D.data[d]; // Copy it to the appropriate place in D2.
//         } 
//         /// BEST
//     }

#pragma omp parallel num_threads(numt)
    {
        int tid = omp_get_thread_num();
         for (uint d = tid * nData / numt; d < ((tid + 1) * nData) / numt; d+=STRIDE) {
              for (uint i = d; i < std::min(d+STRIDE, ((tid + 1) * nData) / numt); i++)                                               
              {                                                                                                                          
                  int r = D.data[i].value;                                                                                                
                  assert(r > 0);                                                                                                          
                  D2.data[rangecount[r-1]+freq[r]++] = D.data[i];                                                                      
              } 
         }
        /// BEST
    }

    return D2;
}
