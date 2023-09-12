#include <bits/stdc++.h> 

void heapify(vector<int>&heap, int heapSize, int rootIndex) {
    if(rootIndex < 0)return;
    int smallestIndex = rootIndex;
    int leftChildIndex = 2 * rootIndex + 1;
    int rightChildIndex = 2 * rootIndex + 2;

    if (leftChildIndex < heapSize && heap[leftChildIndex] < heap[smallestIndex]) {
        smallestIndex = leftChildIndex;
    }
    if (rightChildIndex < heapSize && heap[rightChildIndex] < heap[smallestIndex]) {
        smallestIndex = rightChildIndex;
    }

    if (rootIndex != smallestIndex) {
        swap(heap[rootIndex], heap[smallestIndex]);
        heapify(heap, heapSize, smallestIndex);
    }
}
vector<int> minHeap(int n, vector<vector<int>>& q) {
    vector<int>heap, ans;
    for(int i = 0; i < n; i++){
        int choice = q[i][0];
        if(choice == 0){
            heap.push_back(q[i][1]);
            continue;
        }
        for (int i = heap.size() / 2 - 1; i >= 0; i--)
            heapify(heap, heap.size(), i);
        ans.push_back(heap.front());
        swap(heap.front(), heap.back());
        heap.pop_back();
    }
    return ans;
}
