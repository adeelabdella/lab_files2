#include <bits/stdc++.h>
#include <chrono>
#include <random>
using namespace std;
using namespace chrono;

class SortingAlgorithm
{
public:
    void counting_sort(vector<int> &nums);
    void merge(vector<int> &nums, int left, int mid, int right);
    void merge_sort_divide(vector<int> &nums, int left, int right);
    void merge_sort(vector<int> &nums);
    void radix_sort(vector<int> &nums);
    void heapify(vector<int> &nums, int heapSize, int root);
    void heap_sort(vector<int> &nums);
};

void SortingAlgorithm::merge_sort(vector<int> &nums)
{
    merge_sort_divide(nums, 0, nums.size() - 1);
}

void SortingAlgorithm::merge_sort_divide(vector<int> &nums, int left, int right)
{
    if (left >= right)
        return;
    int mid = left + (right - left) / 2;
    merge_sort_divide(nums, left, mid);
    merge_sort_divide(nums, mid + 1, right);
    merge(nums, left, mid, right);
}
void SortingAlgorithm::merge(vector<int> &nums, int left, int mid, int right)
{
    int start1 = left, start2 = mid + 1;
    int n1 = mid - left + 1, n2 = right - mid;
    vector<int> leftArr(n1);
    vector<int> rightArr(n2);
    for (int i = 0; i < n1; i++)
    {
        leftArr[i] = nums[i + start1];
    }
    for (int i = 0; i < n2; i++)
    {
        rightArr[i] = nums[i + start2];
    }
    int i = 0, j = 0;
    while (i < n1 && j < n2)
    {
        if (leftArr[i] < rightArr[j])
            nums[left++] = leftArr[i++];

        else
            nums[left++] = rightArr[j++];
    }
    while (i < n1)
        nums[left++] = leftArr[i++];
    while (j < n2)
        nums[left++] = rightArr[j++];
}
void SortingAlgorithm::counting_sort(vector<int> &arr)
{
    unordered_map<int, int> counts;
    int minVal = arr[0];
    int maxVal = arr[0];
    for (int &i : arr)
    {
        minVal = min(minVal, i);
        maxVal = max(maxVal, i);
    }
    for (int &val : arr)
        ++counts[val];
    int index = 0;
    for (int val = minVal; val <= maxVal; ++val)
    {
        if (counts.find(val) != counts.end())
        {
            while (counts[val])
            {
                arr[index++] = val;
                counts[val] -= 1;
            }
        }
    }
}
void SortingAlgorithm::radix_sort(vector<int> &arr)
{
    int n = arr.size();
    vector<queue<int>> vs(10);
    long int divisor = 1;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int index = (abs(arr[j]) / divisor) % 10;
            vs[index].push(arr[j]);
        }
        int idx = 0, index = 0;
        while (idx < 10)
        {
            while (!vs[idx].empty())
            {
                arr[index++] = vs[idx].front();
                vs[idx].pop();
            }
            idx++;
        }
        divisor *= 10;
    }
    vector<int> positives, negatives;
    for (int &val : arr)
    {
        if (val < 0)
            negatives.push_back(val);
        else
            positives.push_back(val);
    }
    reverse(negatives.begin(), negatives.end());
    int idx = 0;
    for (int &i : negatives)
        arr[idx++] = i;
    for (int &i : positives)
        arr[idx++] = i;
}
void SortingAlgorithm::heapify(vector<int> &arr, int heapSize, int root)
{
    int largest = root;
    int left = 2 * root + 1, right = 2 * root + 2;
    if (left < heapSize && arr[left] > arr[largest])
        largest = left;
    if (right < heapSize && arr[right] > arr[largest])
        largest = right;
    if (largest != root)
    {
        swap(arr[root], arr[largest]);
        heapify(arr, heapSize, largest);
    }
}
void SortingAlgorithm::heap_sort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i >= 0; i--)
    {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
vector<int> generateRandomData(int size)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, size * 10);

    vector<int> data;
    for (int i = 0; i < size; i++)
    {
        data.push_back(dist(gen));
    }
    return data;
}

template <typename SortingFun>
long long measureSortingTime(SortingFun sortingFunc, vector<int> &data)
{
    auto start = high_resolution_clock::now();
    sortingFunc(data);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count();
}

int main()
{
    // const vector<int> dataSizes = {1000, 5000, 10000, 20000, 100000, 1000000, 10000000};
    // SortingAlgorithm obj;
    vector<int> data = generateRandomData(50);

    // cout << "Testing Sorting Algorithms:" << endl;

    // // for (int dataSize : dataSizes)
    // {
    //     // vector<int> data = generateRandomData(dataSize);
    //     // vector<int> data(dataSize);
    //     // iota(data.begin(), data.end(), 0);
    //     // vector<int> mergeSortData = data;
    //     // vector<int> countingSortData = data;
    //     // vector<int> radixSortData = data;
    //     // vector<int> heapSortData = data;

    //     // long long mergeSortTime = measureSortingTime([&](vector<int> &data)
    //     //                                              { obj.merge_sort(data); },
    //     //                                              mergeSortData);

    //     // long long countingSortTime = measureSortingTime([&](vector<int> &data)
    //     //                                                 { obj.counting_sort(data); },
    //     //                                                 countingSortData);

    //     // long long radixSortTime = measureSortingTime([&](vector<int> &data)
    //     //                                              { obj.radix_sort(data); },
    //     //                                              radixSortData);

    //     // long long heapSortTime = measureSortingTime([&](vector<int> &data)
    //     //                                             { obj.heap_sort(data); },
    //     //                                             heapSortData);

    //     // cout << "Sorting Algorithms for " << dataSize << " elements:" << endl;
    //     // cout << "Merge Sort: " << mergeSortTime << " microseconds" << endl;
    //     // cout << "Counting Sort: " << countingSortTime << " microseconds" << endl;
    //     // cout << "Radix Sort: " << radixSortTime << " microseconds" << endl;
    //     // cout << "Heap Sort: " << heapSortTime << " microseconds" << endl;
    //     // cout << endl;
    // }

    return 0;
}
