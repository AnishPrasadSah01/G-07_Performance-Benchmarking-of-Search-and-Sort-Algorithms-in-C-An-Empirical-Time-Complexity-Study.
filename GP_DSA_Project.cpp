/*
 * Sorting and Searching Algorithm Benchmark
 * This program compares the performance of;
 * Merge Sort and Quick Sort,
 * Binary Search, and Exponential Search algorithms.
 */

#include <iostream>         // It is used for input/output operations
#include <vector>           // It is used for using vector container
#include <chrono>           // It is used for high-resolution timing measured in Nanosecond
#include <random>           // It is used for random number generation
#include <algorithm>        // It is used for built-in sort and other algorithms
#include <limits>           // It is used for numeric limits
#include <cassert>          // It is used for assertions
using namespace std;
using namespace std::chrono;

// ======== Forward Declarations ========
void mergeSort(vector<int>& arr, int sta, int endd);
void quickSort(vector<int>& arr, int sta, int endd);
vector<int> binarySearchAll(const vector<int>& arr, int targett);  
vector<int> exponentialSearchAll(const vector<int>& arr, int targett);
void printArray(const vector<int>& arr, const string& labell);
void measurePerformance(int n);
int getValidatedInput(const string& promptt, int minnVal, int maxxVal);
bool isSorted(const vector<int>& arr);

/*
 * Parameters used in this overall program :-
 *   arr -   It is reference to the vector to be sorted
 *   start - It is represented by 'sta' and indicates starting index of first subarray
 *   medium - It is represented by 'midd' and indicates ending index of first subarray
 *   end -    It is represented by 'endd' and indicates ending index of second subarray
 *   low -    It is represented by 'loww'
 *   'i' and 'j' - Are two iterators which is represented by 'p' and 'q'
 *   target - It is represented by 'targett' and is an target element to be found.
 *   prompt - It is represented by "promptt' and it message to display to user
 *   minVal - It is represented by 'minnVal' and is the minimum acceptable value
 *   maxVal - It is represented by 'maxxVal' and is the maximum acceptable value
 *   label -  It is represented by 'labell' and is a description of the vector
 */


// ================== MERGE SORT IMPLEMENTATION ==================
void merge(vector<int>& arr, int sta, int midd, int endd) {
    vector<int> temp;                      // It is an Temporary vector to store merged result
    int p = sta, q = midd + 1;            // p and q are two iterators for the two subarrays
    
    // Merging the two subarrays while both have elements
    while (p <= midd && q <= endd) {
        if (arr[p] <= arr[q]) {
            temp.push_back(arr[p++]);      // It takes the element from left subarray
        }
        else {
            temp.push_back(arr[q++]);      // It takes the element from right subarray
        }
    }
    
    // It Copies remaining elements from left subarray
    while (p <= midd) {
        temp.push_back(arr[p++]);
    }
    
    // It Copies remaining elements from right subarray
    while (q <= endd) {
        temp.push_back(arr[q++]);
    }
    
    // It Copies merged elements back to the original array
    for (int idx = 0; idx < temp.size(); idx++) {
        arr[idx + sta] = temp[idx];
    }
}

// MergeSort algorithm 
void mergeSort(vector<int>& arr, int sta, int endd) {
    if (sta < endd) {
        int midd = sta + (endd - sta) / 2;  // It calculate middle index and helps to prevent overflow...

        // It recursively sort both halves
        mergeSort(arr, sta, midd);
        mergeSort(arr, midd + 1, endd);
        
        // It merge the sorted halves
        merge(arr, sta, midd, endd);
    }
}

// ================== QUICK SORT IMPLEMENTATION ==================
int partition(vector<int>& arr, int sta, int endd) {
    // Here Median-of-three pivot selection is being used
    int midd = sta + (endd - sta) / 2;
    if (arr[sta] > arr[midd]) {
        swap(arr[sta], arr[midd]);
    }
    if (arr[midd] > arr[endd]) {
        swap(arr[midd], arr[endd]);
    }
    if (arr[sta] > arr[midd]) {
        swap(arr[sta], arr[midd]);
    }    
    int pivot = arr[midd];      // Use pivot = middle element
    swap(arr[midd], arr[endd]); // Move pivot to end
    
    int p = sta - 1;            // It is index of smaller element
    
    // It is Partitioning loop
    for (int q = sta; q < endd; q++) {
        if (arr[q] <= pivot) {
            p++;
            swap(arr[p], arr[q]);  // Here,Swapping of current element with pivot position takes place
        }
    }
    
    // It helps to place pivot in correct position
    swap(arr[p+1], arr[endd]);
    return p+1;  // It simply returns pivot index
}

// QuickSort algorithm
void quickSort(vector<int>& arr, int sta, int endd) {
    if (sta < endd) {
        // It helps in Partition the array and get pivot index
        int pivIdx = partition(arr, sta, endd);
        
        // It recursively sort elements before and after pivot
        quickSort(arr, sta, pivIdx - 1);
        quickSort(arr, pivIdx + 1, endd);
    }
}

// ================== BINARY SEARCH IMPLEMENTATION ==================
vector<int> binarySearchAll(const vector<int>& arr, int targett, int sta, int endd) {
    vector<int> indices;        // It Stores the found indices
    int first = -1, last = -1; // It helps to track the first and last occurrences

    // Finding the first occurrence of target
    int loww = sta, highh = endd;
    while (loww <= highh) {
        int midd = loww + (highh - loww) / 2;  // It Prevents overflow
        if (arr[midd] == targett) {
            first = midd;
            highh = midd - 1;   // It helps to Search left for earlier occurrences
        } else if (arr[midd] < targett) {
            loww = midd + 1;    // It helps to  Search right half
        } else {
            highh = midd - 1;   // It helps to Search left half
        }
    }

    if (first == -1) return indices;  // It represent Target was not found

    // Finding the last occurrence of target
    loww = sta, highh = endd;
    while (loww <= highh) {
        int midd = loww + (highh - loww) / 2;
        if (arr[midd] == targett) {
            last = midd;
            loww = midd + 1;   // It helps to search right for later occurrences
        } else if (arr[midd] < targett) {
            loww = midd + 1;    // It helps to search right half
        } else {
            highh = midd - 1;   // It helps to search left half
        }
    }

    // This collect all indices between first and last occurrence
    for (int p = first; p <= last; ++p)
        indices.push_back(p);
    return indices;
}

// Wrapper function for binary search
vector<int> binarySearchAll(const vector<int>& arr, int targett) {
    if (!isSorted(arr)) return {};  // We know that binary search requires sorted array
    return binarySearchAll(arr, targett, 0, arr.size() - 1);
}

// ================== EXPONENTIAL SEARCH IMPLEMENTATION ==================
vector<int> exponentialSearchAll(const vector<int>& arr, int targett) {
    if (!isSorted(arr) || arr.empty()) return {};  // It simply checks the preconditions
    
    // It handle case where target is first element
    if (arr[0] == targett)
        return binarySearchAll(arr, targett, 0, arr.size() - 1);

    // It finds the range by exponential increase
    int p = 1;
    while (p < arr.size() && arr[p] <= targett)
        p *= 2;

    // It performs the binary search in the found range
    return binarySearchAll(arr, targett, p / 2, min(p, (int)arr.size() - 1));
}

// ================== HELPER FUNCTIONS ==================
/*
 * It helps to prin the contents of a vector with a label
 * Parameters:
 *   arr - it is vector to print
 *   labell - it is a description of the vector
 */
void printArray(const vector<int>& arr, const string& labell) {
    cout << labell << ": [";
    for (size_t p = 0; p < arr.size(); p++) {
        cout << arr[p];
        if (p < arr.size() - 1) cout << ", ";
    }
    cout << "]\n";
}

/*
 * To Gets validated integer input from user
 * Parameters:
 *   promptt - message to display to user
 *   minnVal - minimum acceptable value
 *   maxxVal - maximum acceptable value
 * Returns:
 *   Validated user input
 */
int getValidatedInput(const string& promptt, int minnVal, int maxxVal) {
    int input;
    while (true) {
        cout << promptt;
        cin >> input;
        if (cin.fail() || input < minnVal || input > maxxVal) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter an integer between " 
                 << minnVal << " and " << maxxVal << ".\n";
        } else break;
    }
    return input;
}

/*
 * This checks if a vector is sorted in non-decreasing order
 * Parameters:
 *   arr - it is vector to be checked
 * Returns:
 *   true if sorted, false otherwise
 */
bool isSorted(const vector<int>& arr) {
    return is_sorted(arr.begin(), arr.end());
}

// ================== PERFORMANCE MEASUREMENT ==================
/*
 * It simply Measure and compare the performance of sorting and searching algorithms
 * Parameters:
 *   n - is the size of array to test
 */
void measurePerformance(int n) {
    vector<int> results;
    
    // Initialization of random number generation
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, n * 2);

    // Creating the random array
    vector<int> arr(n);
    for (int p = 0; p < n; p++) arr[p] = dist(gen);

    cout << "\n===== Performance for Input Size (N = " << n << ") =====\n";

    // Here, Warm-up runs to stabilize performance
    for (int p = 0; p < 3; p++) {
        vector<int> warmup = arr;
        mergeSort(warmup, 0, warmup.size() - 1);
    }

    // Measuring the Merge Sort performance
    vector<int> arrMerge = arr;
    auto start = high_resolution_clock::now();
    mergeSort(arrMerge, 0, arrMerge.size() - 1);
    auto stop = high_resolution_clock::now();
    cout << "Time taken by Merge Sort :- " << duration_cast<nanoseconds>(stop - start).count() << " ns\n";

    // Measuring the Quick Sort performance
    vector<int> arrQuick = arr;
    start = high_resolution_clock::now();
    quickSort(arrQuick, 0, arrQuick.size() - 1);
    stop = high_resolution_clock::now();
    cout << "Time taken by Quick Sort :- " << duration_cast<nanoseconds>(stop - start).count() << " ns\n";

    // Verifing the sorting was successful
    if (!isSorted(arrMerge)) cerr << "Merge Sort failed !\n";
    if (!isSorted(arrQuick)) cerr << "Quick Sort failed !\n";

    // Getting target for the search operations
    int targett = getValidatedInput("Enter Target number (0 -" + to_string(n * 2) + ") :- ", 0, n * 2);
    
    // Measuring the Binary Search performance
    start = high_resolution_clock::now();
    results = binarySearchAll(arrMerge, targett);
    stop = high_resolution_clock::now();
    cout << "Time taken by Binary Search   :- " << duration_cast<nanoseconds>(stop - start).count() << " ns\n";
    cout << "   Target " << targett << " found at indices :- ";
    if (results.empty()) {
        cout << "Sorry, the target was not found in any indices of the array.";
    } else {
        for (size_t p = 0; p < results.size(); ++p) {
            cout << results[p];
            if (p < results.size() - 1) cout << ", ";
        }
    }
    cout << "\n";

    // Measuring the Exponential Search performance
    start = high_resolution_clock::now();
    results = exponentialSearchAll(arrMerge, targett);
    stop = high_resolution_clock::now();
    cout << "Time taken by Exponential Search :- " << duration_cast<nanoseconds>(stop - start).count() << " ns\n";
    cout << "   Target " << targett << " found at indices :- ";
    if (results.empty()) {
        cout << "Sorry, the target was not found in any indices of the array.";
    } else {
        for (size_t p = 0; p < results.size(); ++p) {
            cout << results[p];
            if (p < results.size() - 1) cout << ", ";
        }
    }
    cout << "\n";
}

// ================== MAIN FUNCTION ==================
int main() {
    cout << "===== Sorting and Searching Algorithm Benchmark =====\n";
    // Getting array size from the user (validated only between 1 and 10,000,000)
    int n = getValidatedInput("Enter Array size (1 - 10,000,000) :- ", 1, 10'000'000);
    
    // It helps to Run the performance measurements
    measurePerformance(n);
    return 0;
}