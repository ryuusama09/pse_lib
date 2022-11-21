# Parse

Parse is a helpful open-source library for C++. Consisting of many features that are part of the C++ Standard Library
Here are some examples

```C++
template<typename T>
struct Vector{};
```

```C++
template<typename T, typename... Ts>
struct Tuple{};
```

```C++
template<typename T, typename... Ts>
struct TupleLike
{
  template<typename T>
  auto push_back(B b);
}; // Just Tuple combined with a vector
```

```C++
template<typename T>
auto swap(T& a, T& b)
{
  // Implementation
}
```

