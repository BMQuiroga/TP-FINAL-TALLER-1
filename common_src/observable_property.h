#ifndef _OBSERVABLE_PROPERTY_H
#define _OBSERVABLE_PROPERTY_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>

template<typename T>
class IObserver {
 public:
  virtual ~IObserver(){}
  virtual void value_changed(T old_value, T new_value) = 0;
};

template<typename T, typename Owner>
class PropertyObserver {
 protected:
  std::function<void(Owner*, T, T)> callback;
 public:
  explicit PropertyObserver(std::function<void(Owner*, T, T)> callback) : 
  callback(callback) {}
  ~PropertyObserver() {}
  void value_changed(Owner *owner, T old_value, T new_value) {
    callback(owner, old_value, new_value);
  }
};

template<typename T, typename Owner>
class ObservableProperty {
 protected:
    T value;        // the value of this property
    std::vector<PropertyObserver<T, Owner>*> observers;
 public:
  explicit ObservableProperty(T initial) : value(initial) {}
  ~ObservableProperty(){}
  void attach(PropertyObserver<T, Owner> *observer) {
    observers.push_back(observer);
  }
  void detach(PropertyObserver<T, Owner> *observer) {
    observers.remove(observer);
  }
  void notify(Owner *owner, T old_value, T new_value) {
    for (PropertyObserver<T, Owner>* observer : observers) {
        observer->value_changed(owner, old_value, new_value);
    }
  }
  void set(T new_value, Owner *owner) {
    T old = value;
    value = new_value;
    notify(owner, old, value);
  }
  T& get() { return value; }
};
#endif
