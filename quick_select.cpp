class Solution {
public:
  int findKthLargest(vector<int>& nums, int k) {
    // int n = nums.size();
    // if(k == 1)return *max_element(nums.begin(), nums.end());
    // if(k == n)return *min_element(nums.begin(), nums.end());
    // priority_queue <int, vector<int>, greater<int> > pq;
    // for(int i = 0; i < n; i++){
    //   pq.push(nums[i]);
    //   while(pq.size() > k)pq.pop();
    // }
    // return pq.top();
    return reverseQuickSelect(nums, k);
  }

  int reverseQuickSelect(vector<int>&nums, int k){
    int pivot = nums[rand() % nums.size()];

    vector<int>left, mid, right;

    for(int num:nums){
      if(num > pivot)left.push_back(num);
      else if(num < pivot)right.push_back(num);
      else mid.push_back(num);
    }

    if(left.size() >= k){
      return reverseQuickSelect(left, k);
    }
    if(left.size() + mid.size() < k){
      return reverseQuickSelect(right, k - left.size() - mid.size());
    }
    return pivot;
  }

};
