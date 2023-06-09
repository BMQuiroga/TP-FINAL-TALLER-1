#ifndef _OBSERVABLE_PROPERTY_H
#define _OBSERVABLE_PROPERTY_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>

template<typename T>
class IObserver {
 public:
  virtual ~IObserver(){};
  virtual void value_changed(T old_value, T new_value) = 0;
};

template<typename T>
class ObservableProperty {
 protected:
    T value;
    std::vector<IObserver<T>*> observers;
 public:
  ObservableProperty(T initial) : value(initial) {}
  ~ObservableProperty(){};
  void attach(IObserver<T> *observer) {
    observers.push_back(observer);
  }
  void detach(IObserver<T> *observer) {
    observers.remove(observer);
  }
  void notify(T old_value, T new_value) {
    for (IObserver<T>* observer : observers) {
        observer->value_changed(old_value, new_value);
    }
  }
  void set(T new_value) {
    T old = value;
    value = new_value;
    notify(old, value);
  }
  T& get() { return value; }
};

template<typename T>
class PropertyObserver : public IObserver<T> {
 protected:
  std::function<void(T, T)> callback;
 public:
  PropertyObserver(std::function<void(T, T)> callback) : 
  callback(callback) {}
  ~PropertyObserver() override {};
  void value_changed(T old_value, T new_value) override {
    callback(old_value, new_value);
  }
};
#endif
