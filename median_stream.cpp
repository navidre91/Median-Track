#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

void print_vector(vector<int> V){
    for (int i=0; i<V.size(); i++){
        cout<<endl<<V[i];
    }
}


class Median_Holder{

    public:
    string file_address;
    vector<int> nums;
    vector<int> lower_max_heap;
    vector<int> upper_min_heap;
    vector<int> medians;
    int max_lower;
    int min_upper;
    int ans = 0;

    Median_Holder(string f_address){
        file_address = f_address;
        set_nums_vector();
    }

    void set_nums_vector(){
        string num_s;
        int num;
        ifstream infile;
        infile.open(file_address);

        while(!infile.eof()){
            getline(infile,num_s);
            num = stoi(num_s);
            nums.push_back(num);
        }
    }

    void add_max_heap(vector<int> &V, int num){
        V.push_back(num);
        
        bool flag = true;
        int current_loc = V.size();

        while(flag){
            int parent = current_loc/2;
            if (parent>0){
                if (V[(parent-1)]<V[(current_loc-1)]){
                    int temp = V[(parent-1)];
                    V[(parent-1)] = V[(current_loc-1)];
                    V[(current_loc-1)] = temp;
                    current_loc = parent;
                }else{
                    flag = false;
                }
            }else{
                flag = false;
            }
        }

    }
    int extract_max_heap(vector<int> &V){
        int max = V[0];
        int len = V.size();
        V[0] = V[len-1];
        V.pop_back();
        len--;
        if(len == 0 ){
            return max;
        }

        int current_loc = 1;
        bool flag = true;
        if(2*current_loc>len){
            flag = false;
        }

        while(flag){
            if(2*current_loc==len){
                int child = V[(len-1)];
                int parent = V[(current_loc-1)];
                if (child>parent){
                    int temp = V[(len-1)];
                    V[(len-1)] = V[(current_loc-1)];
                    V[(current_loc-1)] = temp; 
                    current_loc = 2*current_loc;
                }else{
                    flag = false;
                }
            }else{
                int parent = V[(current_loc-1)];
                int child1 = V[(2*current_loc-1)];
                int child2 = V[(2*current_loc)];
                int selected_child_ind;
                int child;

                if(child1>child2){
                    selected_child_ind = 2*current_loc;
                    child = child1;
                }else{
                    selected_child_ind = 2*current_loc+1;
                    child = child2;
                }

                if (child>parent){
                    int temp = V[selected_child_ind-1];
                    V[selected_child_ind-1] = V[current_loc-1];
                    V[current_loc-1] = temp;
                    current_loc = selected_child_ind;
                }else{
                    flag = false;
                }


            }
            if(2*current_loc>len){
            flag = false;
            }
        }


        return max;

    }


    void add_min_heap(vector<int> &V, int num){
        V.push_back(num);
        
        bool flag = true;
        int current_loc = V.size();

        while(flag){
            int parent = current_loc/2;
            if (parent>0){
                if (V[(parent-1)]>V[(current_loc-1)]){
                    int temp = V[(parent-1)];
                    V[(parent-1)] = V[(current_loc-1)];
                    V[(current_loc-1)] = temp;
                    current_loc = parent;
                }else{
                    flag = false;
                }
            }else{
                flag = false;
            }
        }

    }
    int extract_min_heap(vector<int> &V){
        int min = V[0];
        int len = V.size();
        V[0] = V[len-1];
        V.pop_back();
        len--;
        if(len == 0 ){
            return min;
        }

        int current_loc = 1;
        bool flag = true;
        if(2*current_loc>len){
            flag = false;
        }

        while(flag){
            if(2*current_loc==len){
                int child = V[(len-1)];
                int parent = V[(current_loc-1)];
                if (child<parent){
                    int temp = V[(len-1)];
                    V[(len-1)] = V[(current_loc-1)];
                    V[(current_loc-1)] = temp; 
                    current_loc = 2*current_loc;
                }else{
                    flag = false;
                }
            }else{
                int parent = V[(current_loc-1)];
                int child1 = V[(2*current_loc-1)];
                int child2 = V[(2*current_loc)];
                int selected_child_ind;
                int child;

                if(child1<child2){
                    selected_child_ind = 2*current_loc;
                    child = child1;
                }else{
                    selected_child_ind = 2*current_loc+1;
                    child = child2;
                }

                if (child<parent){
                    int temp = V[selected_child_ind-1];
                    V[selected_child_ind-1] = V[current_loc-1];
                    V[current_loc-1] = temp;
                    current_loc = selected_child_ind;
                }else{
                    flag = false;
                }


            }
            if(2*current_loc>len){
            flag = false;
            }
        }


        return min;

    }

    void track_median(){
        lower_max_heap.push_back(nums[0]);
        medians.push_back(nums[0]);
        ans = medians[0];
        for (int i=1; i<nums.size(); i++){
            int num_to_add = nums[i];

            if(lower_max_heap.size() > upper_min_heap.size() ){
                if (lower_max_heap[0] <= num_to_add){
                    add_min_heap(upper_min_heap,num_to_add);
                }else if(num_to_add < lower_max_heap[0]){
                    int to_go_to_upper_heap = lower_max_heap[0];
                    add_min_heap(upper_min_heap,to_go_to_upper_heap);
                    lower_max_heap.push_back(num_to_add);
                    extract_max_heap(lower_max_heap);
                }
            }else if(lower_max_heap.size() < upper_min_heap.size()){
                if (num_to_add<=upper_min_heap[0]){
                    add_max_heap(lower_max_heap,num_to_add);
                } else if(num_to_add>upper_min_heap[0]){
                    int to_go_to_lower_heap = upper_min_heap[0];
                    add_max_heap(lower_max_heap,to_go_to_lower_heap);
                    upper_min_heap.push_back(num_to_add);
                    extract_min_heap(upper_min_heap);
                }

            }else if(lower_max_heap.size() == upper_min_heap.size()){
                if (lower_max_heap[0]>=num_to_add){
                    add_max_heap(lower_max_heap,num_to_add);
                }else{
                    add_min_heap(upper_min_heap,num_to_add);
                }
            }

            if(lower_max_heap.size() == upper_min_heap.size()){
                int median_to_add = lower_max_heap[0];
                medians.push_back(median_to_add);
                ans = ans + median_to_add;
            }
            if(lower_max_heap.size() > upper_min_heap.size()){
                int median_to_add = lower_max_heap[0];
                medians.push_back(median_to_add);
                ans = ans + median_to_add;                
            }
            if(lower_max_heap.size() < upper_min_heap.size()){
                int median_to_add = upper_min_heap[0];
                medians.push_back(median_to_add);   
                ans = ans + median_to_add;             
            }
        }
    }

};


int main(){
    string file_address = "G:\\My Drive\\course\\Algorithm\\Heap\\MedianData.txt";
    Median_Holder mh(file_address);

    mh.track_median();
    cout<<mh.ans;

    return 0;
}