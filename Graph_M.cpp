#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

class Graph_M {
private:
    struct Vertex {
        unordered_map<string, int> nbrs;
    };

    unordered_map<string, Vertex> vtces;

public:
    Graph_M() {}

    int numVertex() {
        return vtces.size();
    }

    bool containsVertex(const string& vname) {
        return vtces.find(vname) != vtces.end();
    }

    void addVertex(const string& vname) {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    void removeVertex(const string& vname) {
        auto vtx = vtces.find(vname);
        if (vtx != vtces.end()) {
            auto& neighbors = vtx->second.nbrs;
            for (const auto& neighbor : neighbors) {
                auto& nbrVtx = vtces[neighbor.first];
                nbrVtx.nbrs.erase(vname);
            }
            vtces.erase(vtx);
        }
    }

    int numEdges() {
        int count = 0;
        for (const auto& pair : vtces) {
            count += pair.second.nbrs.size();
        }
        return count / 2;
    }

    bool containsEdge(const string& vname1, const string& vname2) {
        auto vtx1 = vtces.find(vname1);
        auto vtx2 = vtces.find(vname2);

        if (vtx1 == vtces.end() || vtx2 == vtces.end() || vtx1->second.nbrs.find(vname2) == vtx1->second.nbrs.end()) {
            return false;
        }

        return true;
    }

    void addEdge(const string& vname1, const string& vname2, int value) {
        auto vtx1 = vtces.find(vname1);
        auto vtx2 = vtces.find(vname2);

        if (vtx1 != vtces.end() && vtx2 != vtces.end() && vtx1->second.nbrs.find(vname2) == vtx1->second.nbrs.end()) {
            vtx1->second.nbrs[vname2] = value;
            vtx2->second.nbrs[vname1] = value;
        }
    }

    void removeEdge(const string& vname1, const string& vname2) {
        auto vtx1 = vtces.find(vname1);
        auto vtx2 = vtces.find(vname2);

        if (vtx1 != vtces.end() && vtx2 != vtces.end() && vtx1->second.nbrs.find(vname2) != vtx1->second.nbrs.end()) {
            vtx1->second.nbrs.erase(vname2);
            vtx2->second.nbrs.erase(vname1);
        }
    }

    void displayMap() {
        cout << "\t Delhi Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------" << endl;
        vector<string> keys;
        for (const auto& pair : vtces) {
            keys.push_back(pair.first);
        }

        for (const string& key : keys) {
            cout << key << " =>" << endl;
            const auto& vtx = vtces[key];
            vector<string> vtxnbrs;
            for (const auto& nbr : vtx.nbrs) {
                vtxnbrs.push_back(nbr.first);
            }

            for (const string& nbr : vtxnbrs) {
                cout << "\t" << nbr << "\t";
                if (nbr.length() < 16)
                    cout << "\t";
                if (nbr.length() < 8)
                    cout << "\t";
                cout << vtx.nbrs[nbr] << endl;
            }
        }
        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------" << endl;
    }

    void displayStations() {
        cout << "\n***********************************************************************\n";
        vector<string> keys;
        for (const auto& pair : vtces) {
            keys.push_back(pair.first);
        }

        for (int i = 1; i <= keys.size(); i++) {
            cout << i << ". " << keys[i - 1] << endl;
        }

        cout << "\n***********************************************************************\n";
    }

    bool hasPath(const string& vname1, const string& vname2, unordered_map<string, bool>& processed) {
        if (containsEdge(vname1, vname2)) {
            return true;
        }

        processed[vname1] = true;

        const auto& vtx = vtces[vname1];
        vector<string> nbrs;
        for (const auto& nbr : vtx.nbrs) {
            nbrs.push_back(nbr.first);
        }

        for (const string& nbr : nbrs) {
            if (!processed.count(nbr) || !processed[nbr]) {
                if (hasPath(nbr, vname2, processed)) {
                    return true;
                }
            }
        }

        return false;
    }

    struct DijkstraPair {
        string vname;
        string psf;
        int cost;

        bool operator<(const DijkstraPair& other) const {
            return cost > other.cost;
        }
    };

    int dijkstra(const string& src, const string& des, bool nan) {
        int val = 0;
        vector<string> ans;
        unordered_map<string, DijkstraPair> map;

        priority_queue<DijkstraPair> heap;

        for (const auto& pair : vtces) {
            DijkstraPair np;
            np.vname = pair.first;
            np.cost = INT_MAX;

            if (pair.first == src) {
                np.cost = 0;
                np.psf = pair.first;
            }

            heap.push(np);
            map[pair.first] = np;
        }

        while (!heap.empty()) {
            DijkstraPair rp = heap.top();
            heap.pop();

            if (rp.vname == des) {
                val = rp.cost;
                break;
            }

            map.erase(rp.vname);

            ans.push_back(rp.vname);

            const Vertex& v = vtces[rp.vname];
            for (const auto& nbr : v.nbrs) {
                if (map.count(nbr.first)) {
                    int oc = map[nbr.first].cost;
                    const Vertex& k = vtces[rp.vname];
                    int nc;

                    if (nan) {
                        nc = rp.cost + 120 + 40 * k.nbrs.at(nbr.first);
                    } else {
                        nc = rp.cost + k.nbrs.at(nbr.first);
                    }

                    if (nc < oc) {
                        DijkstraPair& gp = map[nbr.first];
                        gp.psf = rp.psf + nbr.first;
                        gp.cost = nc;

                        heap.push(gp);
                    }
                }
            }
        }

        return val;
    }

    struct Pair {
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };

    string multiSolver(const string& src, const string& des, bool nan) {
        int val_d = 0;
        int val_t = 0;
        string path_d;
        string path_t;

        priority_queue<Pair> heap;
        unordered_map<string, Pair> map;

        for (const auto& pair : vtces) {
            Pair np;
            np.vname = pair.first;
            np.min_dis = INT_MAX;
            np.min_time = INT_MAX;

            if (pair.first == src) {
                np.min_dis = 0;
                np.min_time = 0;
                np.psf = pair.first;
            }

            heap.push(np);
            map[pair.first] = np;
        }

        while (!heap.empty()) {
            Pair rp = heap.top();
            heap.pop();

            map.erase(rp.vname);

            const Vertex& v = vtces[rp.vname];
            for (const auto& nbr : v.nbrs) {
                if (map.count(nbr.first)) {
                    int oc_dis = map[nbr.first].min_dis;
                    int oc_time = map[nbr.first].min_time;
                    const Vertex& k = vtces[rp.vname];
                    int nc_dis;
                    int nc_time;

                    if (nan) {
                        nc_dis = rp.min_dis + 1;
                        nc_time = rp.min_time + 120 + 40 * k.nbrs.at(nbr.first);
                    } else {
                        nc_dis = rp.min_dis + 1;
                        nc_time = rp.min_time + k.nbrs.at(nbr.first);
                    }

                    if (nc_dis < oc_dis || (nc_dis == oc_dis && nc_time < oc_time)) {
                        Pair& gp = map[nbr.first];
                        gp.psf = rp.psf + nbr.first;
                        gp.min_dis = nc_dis;
                        gp.min_time = nc_time;

                        heap.push(gp);

                        if (nbr.first == des) {
                            val_d = gp.min_dis;
                            val_t = gp.min_time;
                            path_d = gp.psf;
                            path_t = gp.psf;
                        }
                    }
                }
            }
        }

        string result;
        result += "Shortest Path in terms of distance: " + path_d + ", Distance: " + to_string(val_d) + " stations\n";
        result += "Shortest Path in terms of time: " + path_t + ", Time: " + to_string(val_t) + " minutes\n";

        return result;
    }
};

int main() {
    Graph_M graph;

    graph.addVertex("Rajiv Chowk");
    graph.addVertex("Kashmere Gate");
    graph.addVertex("Lal Quila");
    graph.addVertex("Jama Masjid");
    graph.addVertex("Chandni Chowk");
    graph.addVertex("New Delhi");
    graph.addVertex("Chawri Bazar");
    graph.addVertex("Central Secretariat");
    graph.addVertex("Patel Chowk");

    graph.addEdge("Rajiv Chowk", "Kashmere Gate", 18);
    graph.addEdge("Rajiv Chowk", "New Delhi", 15);
    graph.addEdge("Rajiv Chowk", "Patel Chowk", 12);
    graph.addEdge("Kashmere Gate", "Lal Quila", 10);
    graph.addEdge("Kashmere Gate", "Chandni Chowk", 8);
    graph.addEdge("Chandni Chowk", "Lal Quila", 12);
    graph.addEdge("Chandni Chowk", "Jama Masjid", 5);
    graph.addEdge("Chandni Chowk", "New Delhi", 15);
    graph.addEdge("New Delhi", "Patel Chowk", 5);
    graph.addEdge("New Delhi", "Central Secretariat", 12);
    graph.addEdge("New Delhi", "Lal Quila", 10);
    graph.addEdge("Central Secretariat", "Patel Chowk", 5);

    cout << "Delhi Metro Map:" << endl;
    graph.displayMap();

    string source, destination;
    bool nano;

    cout << "\nEnter source station: ";
    getline(cin, source);

    cout << "Enter destination station: ";
    getline(cin, destination);

    cout << "Do you want to prioritize travel time over the number of stations (enter 1 for yes, 0 for no)? ";
    cin >> nano;
    cin.ignore();

    cout << graph.multiSolver(source, destination, nano) << endl;

    return 0;
}
