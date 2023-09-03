#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

template <typename T>
class Heap {
private:
    vector<T> data;
    unordered_map<T, int> map;

    void upheapify(int ci) {
        int pi = (ci - 1) / 2;
        if (isLarger(data[ci], data[pi]) > 0) {
            swap(pi, ci);
            upheapify(pi);
        }
    }

    void swap(int i, int j) {
        T ith = data[i];
        T jth = data[j];

        data[i] = jth;
        data[j] = ith;

        map[ith] = j;
        map[jth] = i;
    }

    void downheapify(int pi) {
        int lci = 2 * pi + 1;
        int rci = 2 * pi + 2;
        int maxi = pi;

        if (lci < data.size() && isLarger(data[lci], data[maxi]) > 0) {
            maxi = lci;
        }

        if (rci < data.size() && isLarger(data[rci], data[maxi]) > 0) {
            maxi = rci;
        }

        if (maxi != pi) {
            swap(maxi, pi);
            downheapify(maxi);
        }
    }

public:
    void add(T item) {
        data.push_back(item);
        map[item] = data.size() - 1;
        upheapify(data.size() - 1);
    }

    void display() {
        for (const T& item : data) {
            cout << item << " ";
        }
        cout << endl;
    }

    int size() {
        return data.size();
    }

    bool isEmpty() {
        return size() == 0;
    }

    T remove() {
        swap(0, data.size() - 1);
        T rv = data.back();
        data.pop_back();
        map.erase(rv);
        downheapify(0);
        return rv;
    }

    T get() {
        return data[0];
    }

    int isLarger(T t, T o) {
        if (t < o) {
            return -1;
        } else if (t > o) {
            return 1;
        }
        return 0;
    }

    void updatePriority(T pair) {
        int index = map[pair];
        upheapify(index);
    }
};
