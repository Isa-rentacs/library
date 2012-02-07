/*
Dijkstra
 void dijkstra(int s,vector<Weight> &cost, vector<int> &prev, vector<vector<Edge> > edges);
s:始点ノードの番号(0-index)
cost[i]:sからi-thノードまでの距離を入れるvectorを渡す。
prev[i]:sからの最短路においてsの前のノードのindexを入れるvectorを渡す
edges[i]:i-thノードから出ているedgeのvector

vector<int> createpath(vector<int> &prev, int e)
prev:帰ってきたvectorを渡す
e:到達値
返値:最短経路のindex
*/

typedef double Weight; 

struct Edge{
    int src;
    int dst;
    Weight weight;

    Edge(int src, int dst, Weight weight):
        src(src), dst(dst), weight(weight){ }
};

struct Node{
    int index;
    Weight cost;
    int prev;

    Node(int index, Weight cost, int prev) :
        index(index), cost(cost), prev(prev){ }
};

bool operator <(const Node &a, const Node &b){
    if(a.cost == b.cost){
        return a.index > b.index;
    }else{
        return a.cost > b.cost;
    }
}

void dijkstra(int s, vector<Weight> &costs, vector<int> &prev, vector<vector<Edge> > &edges){
    int n=costs.size();
    priority_queue<Node> q;

    costs.assign(n,INF);
    prev.assign(n,-1);
    costs[s] = 0;
    q.push(Node(s,0,-2));

    while(!q.empty()){
        Node fix = q.top(); 
        q.pop();
        if(prev[fix.index] != -1) continue;
       
        costs[fix.index] = fix.cost;
        prev[fix.index] = fix.prev;

        for(int i=0;i<edges[fix.index].size();i++){ //foreach edge from fixed
            if(costs[edges[fix.index][i].dst] > costs[fix.index] + edges[fix.index][i].weight){
                q.push(Node(edges[fix.index][i].dst, edges[fix.index][i].weight + costs[fix.index], fix.index));
            }
        }
    }
}

vector<int> createpath(vector<int> &prev, int e){
    vector<int> ret;
    int pos = e;
    while(1){
        ret.push_back(pos);
        pos = prev[pos];
        if(pos == -2){
            reverse(ret.begin(),ret.end());
            return ret;
        }
    }
}

/*
 * Binary Indexed Tree
 *
 */

class BIT{
public:
    int *p;
    int num;
    BIT(int arg){
        p = new int[arg+1];
        for(int i=0;i<arg+1;i++) p[i] = 0;
        num = arg;
    }
    
    //return sum of p[1] ... p[arg]
    int sum(int arg){
        int ret=0;
        while(arg > 0){
            ret += p[arg];
            arg -= arg & -arg;
        }
        return ret;
    }

    //add value to index-th node
    void add(int index, int value){
        while(index <= num){
            p[index] += value;
            index += index & -index;
        }
    }
};
