// static int a = 0;
// struct hashFunction {
//     size_t operator()(const pair<int, int>& x) const {
//         size_t h1 = std::hash<int>()(x.first);
//         size_t h2 = std::hash<int>()(x.second);
//         ++a;
//         return h1 ^ (h2 << 1);
//     }
// };

// struct hashFunction{
//     size_t operator()(const pair<int, int> &x) const{
//         // cout<<"Called";
//         ++a;
//         return x.first ^ x.second;
//     }
// };

class Solution {
  public:
    vector<int> maxCombinations(int N, int K, vector<int> &A, vector<int> &B) {
        sort(A.rbegin(), A.rend());
        sort(B.rbegin(), B.rend());
        // unordered_set<pair<int, int>, hashFunction>s;
        set<pair<int, int>>s;
        priority_queue<pair<int, pair<int, int>>>pq;
        pq.push({A[0] + B[0], {0, 0}});
        s.insert({0, 0});
        vector<int>ans;
        for(int count = 0; count < K; count++){
            auto front = pq.top();
            pq.pop();
            ans.push_back(front.first);
            int indexA = front.second.first;
            int indexB = front.second.second;
            if(indexA + 1 < N && s.find({indexA + 1, indexB}) == s.end()){
                pq.push({A[indexA + 1] + B[indexB], {indexA + 1, indexB}});
                s.insert({indexA + 1, indexB});
            }
            if(indexB + 1 < N && s.find({indexA, indexB + 1}) == s.end()){
                pq.push({A[indexA] + B[indexB + 1], {indexA, indexB + 1}});
                s.insert({indexA, indexB + 1});
            }
        }
        return ans;
    }
};
