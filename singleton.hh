#pragma once

template <typename T>
class Singleton {
public:
  static T* instance() {
    if (nullptr == m_value) {
      m_value = new T();
    }

    return m_value;
  }

  Singleton(Singleton&) = delete;
  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&) = delete;
  Singleton& operator=(Singleton&&) = delete;
private:
  Singleton() = default;
  ~Singleton() {
    if (m_value) delete m_value;
  }
private:
  static T* m_value;
};

template <typename T>
T* Singleton<T>::m_value = nullptr;

