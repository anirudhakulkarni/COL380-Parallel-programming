#include "classify.h"
#include <omp.h>
#include <atomic>
#include <chrono>

Data classify(Data &D, const Ranges &R, unsigned int numt)
{ // Classify each item in D into intervals (given by R). Finally, produce in D2 data sorted by interval

   assert(numt < MAXTHREADS);
   std::atomic<int> rcountArray [R.num()];
   
   for (int r = 0; r < R.num(); r++)
    { // For all intervals
        rcountArray[r] = 0;
    }

   Counter counts[R.num()]; // I need on counter per interval. Each counter can keep pre-thread subcount.
   // #pragma omp parallel num_threads(numt)
   // {
   //    int tid = omp_get_thread_num(); // I am thread number tid
   //    for(int i=tid; i<D.ndata; i+=numt) { // Threads together share-loop through all of Data
   //       int v = D.data[i].value = R.range(D.data[i].key);// For each data, find the interval of data's key,
	// 						  // and store the interval id in value. D is changed.
   //       counts[v].increase(tid); // Found one key in interval v
   //    }
   // }
//----------------------------------------------------------------
      auto begin = std::chrono::high_resolution_clock::now();
   
   #pragma omp parallel num_threads(numt)
   {
      int tid = omp_get_thread_num(); // I am thread number tid
      for(int i=tid; i<D.ndata; i+=numt) { // Threads together share-loop through all of Data
          D.data[i].value = R.range(D.data[i].key);// For each data, find the interval of data's key,
							  // and store the interval id in value. D is changed.
         // counts[v].increase(tid); // Found one key in interval v
      }
   }

   auto end = std::chrono::high_resolution_clock::now();
   auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
   std::cout<<elapsed.count()*1e-6<<std::endl;
    #pragma omp parallel num_threads(numt)
   {
      int tid = omp_get_thread_num(); // I am thread number tid
      for(int i=tid; i<D.ndata; i+=numt) { // Threads together share-loop through all of Data
         int v = D.data[i].value;// = R.range(D.data[i].key);// For each data, find the interval of data's key,
							  // and store the interval id in value. D is changed.
         counts[v].increase(tid); // Found one key in interval v
      }
   }


//----------------------------------------------------------------
   // Accumulate all sub-counts (in each interval;'s counter) into rangecount
   unsigned int *rangecount = new unsigned int[R.num()];
   for(int r=0; r<R.num(); r++) { // For all intervals
      rangecount[r] = 0;
      for(int t=0; t<numt; t++) // For all threads
         rangecount[r] += counts[r].get(t);
      // std::cout << rangecount[r] << " elements in Range " << r << "\n"; // Debugging statement
   }

   // Compute prefx sum on rangecount.
   for(int i=1; i<R.num(); i++) {
      rangecount[i] += rangecount[i-1];
   }

   // Now rangecount[i] has the number of elements in intervals before the ith interval.

   Data D2 = Data(D.ndata); // Make a copy
   // auto begin = std::chrono::high_resolution_clock::now();
   
   #pragma omp parallel num_threads(numt)
   {
//----------------------------------------------------------------
      // int tid = omp_get_thread_num();
      // for(int r=tid; r<R.num(); r+=numt) { // Thread together share-loop through the intervals 
      //    int rcount = 0;
      //    for(int d=0; d<D.ndata; d++) // For each interval, thread loops through all of data and  
      //        if(D.data[d].value == r) // If the data item is in this interval 
      //            D2.data[rangecount[r-1]+rcount++] = D.data[d]; // Copy it to the appropriate place in D2.
      // }
//----------------------------------------------------------------
      int tid=omp_get_thread_num();
      int start=tid*D.ndata/numt, end= std::min(((tid + 1) * D.ndata) / numt,D.ndata);
      for(int d=start;d<end;d++){
         int r=D.data[d].value;
         D2.data[rangecount[r-1]+rcountArray[r]++]=D.data[d];
      }
//----------------------------------------------------------------
      // int tid=omp_get_thread_num();
      // int start=tid*D.ndata/numt, end= std::min(((tid + 1) * D.ndata) / numt,D.ndata);
      // for(int d=start;d<end;d++){
      //    int r=D.data[d].value;
      //    D2.data[rangecount[r-1]+rcountArray[r]++]=D.data[d];
      // }

   }
   // auto begin = std::chrono::high_resolution_clock::now();

   // auto end = std::chrono::high_resolution_clock::now();
   // auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
   // std::cout<<elapsed.count()*1e-6<<std::endl;
   return D2;
}
