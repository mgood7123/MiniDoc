#ifndef MINIDOC_CACHE_H
#define MINIDOC_CACHE_H

#include <type_traits>
#include <tuple>
#include <functional>
#include <vector>

namespace MiniDoc {
  
  class CacheBase {
    public:
    
    // intentionally not marked as const
    //  if you need to invalidate
    //   then your probably using the cache wrong
    virtual void invalidate() {};
    virtual ~CacheBase() = default;
  };
  
  class CacheInvalidator : public CacheBase {
    std::vector<CacheBase*> caches;
    public:
    
    CacheInvalidator(const std::initializer_list<CacheBase*> & caches) : caches(caches) {}
    
    // intentionally not marked as const
    //  if you need to invalidate
    //   then your probably using the cache wrong
    void invalidate() override {
      for (CacheBase * cache : caches) {
        cache->invalidate();
      }
    }
    
    CacheInvalidator(const CacheInvalidator&copy) {
      //puts("CacheInvalidator COPY");
    }
    CacheInvalidator(const CacheInvalidator&&move) {
      //puts("CacheInvalidator MOVE");
    }
    CacheInvalidator&operator=(const CacheInvalidator&copy) {
      //puts("CacheInvalidator ASSIGN COPY");
      return *this;
    }
    const CacheInvalidator&operator=(const CacheInvalidator&copy) const {
      //puts("CacheInvalidator ASSIGN COPY CONST");
      return *this;
    }
    CacheInvalidator&operator=(const CacheInvalidator&&move) {
      //puts("CacheInvalidator ASSIGN MOVE");
      return *this;
    }
    const CacheInvalidator&operator=(const CacheInvalidator&&move) const {
      //puts("CacheInvalidator MOVE ASSIGN CONST");
      return *this;
    }
  };
  
  //https://stackoverflow.com/a/754754
  
  template <typename V>
  class CacheNoArgs : public CacheBase {
    mutable V value;
    std::function<V()> getter;
    mutable bool invalidated = true;
    mutable uint32_t hits = 0, invalidated_misses = 0;
    const char* name = "default";
    
    public:
    
    CacheNoArgs() = default;
    
    CacheNoArgs(const char * name, const std::function<V()> & getter) : name(name), getter(getter) {}
    
    V operator()() const {
      return getCacheValue();
    }
    
    V getCacheValue() const {
      if (invalidated) {
        invalidated_misses++;
        value = getter();
        invalidated = false;
        //printf("CacheArgs (name: %s) miss, invalidated\n", name);
      } else {
        hits++;
        //printf("CacheArgs (name: %s) hit\n", name);
      }
      return value;
    }
    
    // intentionally not marked as const
    //  if you need to invalidate
    //   then your probably using the cache wrong
    void invalidate() override {
      invalidated = true;
    }
    
    ~CacheNoArgs() {
      //printf("CacheNoArgs (name: %s) hits: %d, inval misses: %d\n", name == nullptr ? "default" : name, hits, invalidated_misses);
    }
  };
  
  template <typename V, typename ... Args>
  class CacheArgs : public CacheBase {
    mutable V value;
    mutable std::tuple<Args...> dep;
    std::function<V(Args...)> getter;
    mutable bool invalidated = true;
    mutable uint32_t hits = 0, misses = 0, argument_misses = 0, invalidated_misses = 0;
    const char* name = "default";
    
    public:
    
    CacheArgs() = default;
    
    CacheArgs(const char * name, const std::function<V(Args...)> & getter) : name(name), getter(getter) {}
    
    V operator()(const Args & ... args) const {
      return getCacheValue(args...);
    }
    
    V getCacheValue(const Args & ... args) const {
      if (invalidated) {
        dep = {args...};
        value = getter(args...);
        invalidated = false;
        invalidated_misses++;
        //printf("CacheArgs (name: %s) miss, invalidated\n", name);
      } else {
        std::tuple<const Args & ...> a {args...};
        if (dep != a) {
          dep = a;
          value = getter(args...);
          argument_misses++;
          //printf("CacheArgs (name: %s) miss,  different args\n", name);
        } else {
          //printf("CacheArgs (name: %s) hit\n", name);
          hits++;
        }
      }
      return value;
    }
    
    // intentionally not marked as const
    //  if you need to invalidate
    //   then your probably using the cache wrong
    void invalidate() override {
      invalidated = true;
    }
    
    ~CacheArgs() {
      //printf("CacheArgs (name: %s) hits: %d, misses: %d, inval misses: %d\n", name == nullptr ? "default" : name, hits, argument_misses, invalidated_misses);
    }
  };
  
  template <typename V, typename ...Args>
  class Cache : public std::conditional_t<sizeof...(Args) != 0, CacheArgs<V, Args...>, CacheNoArgs<V>>
  {
    using Base = std::conditional_t<sizeof...(Args) != 0, CacheArgs<V, Args...>, CacheNoArgs<V>>;
    public:
    using Base::Base;
    using Base::operator=;
    using Base::operator();
    using Base::getCacheValue;
    using Base::invalidate;
  };
  
  #define MINIDOC_CACHE_FUNC(name, function_name) decltype(MiniDoc::CacheHelper::Get(#function_name, &function_name)) name = MiniDoc::CacheHelper::Get(#function_name, &function_name)
  
  struct CacheHelper {
    template <typename R, typename ... P>
    static auto Get(const char * name, R (*function)(P...)) {
      return Cache<R, P...>(name, function);
    };
    
    template <typename f, typename R, typename ... P>
    static auto Get(const char * name, R (f::*function)(P...)) {
      return Cache<R, f*, P...>(name, function);
    };
    
    // const overload
    template <typename f, typename R, typename ... P>
    static auto Get(const char * name, R (f::*function)(P...) const) {
      return Cache<R, const f*, P...>(name, function);
    };
  };
}
#endif