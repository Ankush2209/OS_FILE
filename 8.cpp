#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

class LRUCache {
public:
    LRUCache(int capacity) : _capacity(capacity) {}

    int get(int key) {
        auto it = _cache.find(key);
        if (it == _cache.end()) return -1;

        // Move accessed page to the front of the list
        _lru.splice(_lru.begin(), _lru, it->second);

        return it->second->second;
    }

    void put(int key, int value) {
        auto it = _cache.find(key);
        if (it != _cache.end()) {
            // Update the value and move the page to the front of the list
            it->second->second = value;
            _lru.splice(_lru.begin(), _lru, it->second);
            return;
        }

        if (_cache.size() >= _capacity) {
            // Remove least recently used page from cache
            int lruKey = _lru.back().first;
            _cache.erase(lruKey);
            _lru.pop_back();
        }

        // Add new page to the cache and to  front of the list
        _lru.emplace_front(key, value);
        _cache[key] = _lru.begin();
    }

private:
    int _capacity;
    list<pair<int, int>> _lru; // List to keep track of the access order
    unordered_map<int, list<pair<int, int>>::iterator> _cache; // Map to quickly look up a page
};

int main() {
    LRUCache cache(2);

    cache.put(1, 1);
    cache.put(2, 2);
    cout << cache.get(1) << endl; // Returns 1
    cache.put(3, 3); // Evicts key 2
    cout << cache.get(2) << endl; // Returns -1 (not found)
    cache.put(4, 4); // Evicts key 1
    cout << cache.get(1) << endl; // Returns -1 (not found)
    cout << cache.get(3) << endl; // Returns 3
    cout << cache.get(4) << endl; // Returns 4

    return 0;
}
