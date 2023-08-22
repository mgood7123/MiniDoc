#ifndef MINIDOC_CACHE_H
#define MINIDOC_CACHE_H

#include <type_traits>
#include <tuple>
#include <vector>
#include <cinttypes>
#include <darcs_types.h> // function

namespace MiniDoc {
  
  class CacheBase {
    public:
    
    // intentionally not marked as const
    //  if you need to invalidate a const cache
    //   then your probably using the cache wrong
    virtual void invalidate() {};
    virtual ~CacheBase() = default;
  };
  
  /*
     example usage

        MINIDOC_CACHE_FUNC(cache_length, AdapterPieceTableWithLineInfo::length);
        MINIDOC_CACHE_FUNC(cache_lines, AdapterPieceTableWithLineInfo::lines);
        MINIDOC_CACHE_FUNC(cache_line_start, AdapterPieceTableWithLineInfo::line_start);
        MINIDOC_CACHE_FUNC(cache_line_end, AdapterPieceTableWithLineInfo::line_end);

        CacheInvalidator caches = [](auto * this_) {
            auto * t = static_cast<AdapterPieceTableWithLineInfo*>(this_);
            return { t->cache_lines, t->cache_length, t->cache_line_start, t->cache_line_end };
        };

  */
  class CacheInvalidator {
    DarcsPatch::function<std::vector<CacheBase*>(void *)> caches;
    public:
    
    template <typename U>
    CacheInvalidator(const U & caches) : caches(caches) {}
    
    CacheInvalidator(const CacheInvalidator & other) {
      caches = other.caches;
    }
    
    const CacheInvalidator & operator=(const CacheInvalidator & other) {
      caches = other.caches;
      return *this;
    }

    // intentionally not marked as const
    //  if you need to invalidate a const cache
    //   then your probably using the cache wrong
    void invalidate(void * instance) {
      auto c = caches(instance);
      for (CacheBase * cache : c) {
        cache->invalidate();
      }
    }

    virtual ~CacheInvalidator() = default;
  };
  
  //https://stackoverflow.com/a/754754
  
  template <typename V>
  class CacheNoArgs : public CacheBase {
    mutable V value;
    DarcsPatch::function<V()> getter;
    mutable bool invalidated = true;
    mutable uint32_t hits = 0, invalidated_misses = 0;
    const char* name = "default";
    
    public:
    
    CacheNoArgs() = default;
    
    CacheNoArgs(const char * name, const DarcsPatch::function<V()> & getter) : name(name), getter(getter) {}

    CacheNoArgs(const CacheNoArgs & other) {
      value = other.value;
      getter = other.getter;
      invalidated = other.invalidated;
      hits = other.hits;
      invalidated_misses = other.invalidated_misses;
      name = other.name;
    }
    
    const CacheNoArgs & operator=(const CacheNoArgs & other) {
      value = other.value;
      getter = other.getter;
      invalidated = other.invalidated;
      hits = other.hits;
      invalidated_misses = other.invalidated_misses;
      name = other.name;
      return *this;
    }

    V operator()() const {
      return getCacheValue();
    }
    
    V & getCacheValue() const {
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
    //  if you need to invalidate a const cache
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
    DarcsPatch::function<V(Args...)> getter;
    mutable bool invalidated = true;
    mutable uint32_t hits = 0, misses = 0, argument_misses = 0, invalidated_misses = 0;
    const char* name = "default";
    
    public:
    
    CacheArgs() = default;
    
    CacheArgs(const char * name, const DarcsPatch::function<V(Args...)> & getter) : name(name), getter(getter) {}
    
    CacheArgs(const CacheArgs & other) {
      value = other.value;
      dep = other.dep;
      getter = other.getter;
      invalidated = other.invalidated;
      hits = other.hits;
      misses = other.misses;
      argument_misses = other.argument_misses;
      invalidated_misses = other.invalidated_misses;
      name = other.name;
    }
    
    const CacheArgs & operator=(const CacheArgs & other) {
      value = other.value;
      dep = other.dep;
      getter = other.getter;
      invalidated = other.invalidated;
      hits = other.hits;
      invalidated_misses = other.invalidated_misses;
      name = other.name;
      return *this;
    }

    V & operator()(const Args & ... args) const {
      return getCacheValue(args...);
    }
    
    V & getCacheValue(const Args & ... args) const {
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
    //  if you need to invalidate a const cache
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