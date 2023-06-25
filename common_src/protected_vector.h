#ifndef _PROTECTED_VECTOR_H
#define _PROTECTED_VECTOR_H

#include <vector>
#include <mutex>
#include <algorithm>

template<typename T>
class ProtectedVector {
  private:
    std::vector<T> vec;
    mutable std::mutex mtx;

  public:
    using base = std::vector<T>;
    using iterator = typename base::iterator;
    using const_iterator = typename base::const_iterator;

    ProtectedVector() = default;
    ProtectedVector(ProtectedVector<T> &&other) : vec(other.vec) {}

    void push_back(T&& value) {
        std::lock_guard<std::mutex> lock(mtx);
        vec.push_back(value);
    }
    
    void push_back(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        vec.push_back(value);
    }

    void pop_back() {
        std::lock_guard<std::mutex> lock(mtx);
        vec.pop_back();
    }

    T& operator[](size_t index) {
        std::lock_guard<std::mutex> lock(mtx);
        return vec[index];
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return vec.size();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return vec.empty();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        vec.clear();
    }
    
    template<typename Predicate>
    void erase(Predicate pred) {
        std::lock_guard<std::mutex> lock(mtx);
        vec.erase(std::remove_if(vec.begin(), vec.end(), pred), vec.end());
    }

    iterator begin() {
        return vec.begin();
    }

    iterator end() {
        return vec.end();
    }

    template<typename Ret = void, typename Func>
    Ret call(Func callback) {
        std::lock_guard<std::mutex> lock(mtx);
        return callback(vec);
    }

    template<typename Ret = void, typename Func>
    Ret for_each(Func callback) {
        std::lock_guard<std::mutex> lock(mtx);
        for (size_t i = 0, n = vec.size(); i < n; i++) {
            T &member = vec[i];
            callback(member, i);
        }
    }
};

#endif
