#ifndef JSON_JSON_H
#define JSON_JSON_H

#include "null.h"

#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace json {

enum class Type {
  null,
  string,
  object,
  integer,
};

class Json {
public:
  using null_t = Null;
  using string_t = std::string;
  using object_t = std::map<string_t, Json>;
  using int_t = std::int64_t;

public: // Constructors
  Json() : m_variant{} {}

  // Json string ctors
  Json(const char *str) : m_variant{str} {}
  Json(string_t &str) : m_variant{str} {}
  Json(string_t &&str) : m_variant{std::forward<string_t>(str)} {}

  // Json object ctors
  Json(std::initializer_list<object_t::value_type> pairs)
      : m_variant{object_t{pairs}} {}
  Json(const object_t &obj) : m_variant{obj} {}
  Json(object_t &&obj) : m_variant{std::forward<object_t>(obj)} {}

  // Json integer ctors
  explicit Json(int_t n) : m_variant{n} {}

public: // Json common operations
  Type type() const { return m_variant.type(); }
  friend std::ostream &operator<<(std::ostream &out, const Json &jsn);

public: // Json string operations
  std::vector<string_t::value_type> chars() const;
  bool isString() const;
  string_t toString() const & {
    assert_string_type();
    return m_variant.str();
  }

  string_t toString() && {
    assert_string_type();
    return m_variant.extract_str();
  }

  operator string_t() const & {
    assert_string_type();
    return m_variant.str();
  }

  operator string_t() && {
    assert_string_type();
    return m_variant.extract_str();
  }

private: // Json string private operations
  void assert_string_type() const;

public: // Json object operations
  std::vector<object_t::key_type> keys() const;
  std::vector<object_t::mapped_type> values() const;
  std::vector<object_t::value_type> items() const;
  friend std::ostream &operator<<(std::ostream &, const object_t &);
  friend std::ostream &operator<<(std::ostream &, const object_t::value_type &);
  bool isObject() const;
  const object_t::mapped_type &operator[](const object_t::key_type &) const;
  void update(std::vector<object_t::value_type> pairs);
  void set(object_t::value_type item);
  Json pop(const object_t::key_type &key);

  object_t toObject() const & {
    assert_object_type();
    return m_variant.object();
  }

  object_t toObject() && {
    assert_object_type();
    return m_variant.extract_object();
  }

  operator object_t() const & {
    assert_object_type();
    return m_variant.object();
  }

  operator object_t() && {
    assert_object_type();
    return m_variant.extract_object();
  }

private: // Json object private operations
  void assert_object_type() const;

public: // Json integer operations
  bool isInteger() const { return (m_variant.type() == Type::integer); }
  int_t toInteger() {
    assert_integer_type();
    return m_variant.integer();
  }

  explicit operator int_t() {
    assert_integer_type();
    return m_variant.integer();
  }

private: // Json integer private operations
  void assert_integer_type() const {
    if (!isInteger()) {
      throw std::runtime_error{"TypeError: not a json integer"};
    }
  }

private:
  struct CoreWrapper {
  private:
    union Core {
      std::unique_ptr<null_t> m_pnull;
      std::unique_ptr<string_t> m_pstr;
      std::unique_ptr<object_t> m_pobj;
      std::unique_ptr<int_t> m_pint;

      Core() : m_pnull{Default<null_t>()} {}

      Core(const char *str) : m_pstr{make<string_t>(str)} {}

      Core(string_t str) : m_pstr{make<string_t>(std::move(str))} {}

      Core(object_t obj) : m_pobj{make<object_t>(std::move(obj))} {}

      Core(int_t n) : m_pint{make<int_t>(n)} {}

      Core &operator=(string_t &&str) {
        constructInner<string_t>(&m_pstr, std::forward<string_t>(str));
        return *this;
      }

      Core &operator=(null_t &&null) {
        constructInner<null_t>(&m_pnull, std::forward<null_t>(null));
        return *this;
      }

      Core &operator=(object_t &&obj) {
        constructInner<object_t>(&m_pobj, std::forward<object_t>(obj));
        return *this;
      }

      Core &operator=(int_t obj) {
        constructInner<int_t>(&m_pint, obj);
        return *this;
      }

      string_t &str() const { return *m_pstr; }

      null_t &null() const { return *m_pnull; }

      object_t &object() const { return *m_pobj; }

      int_t integer() const { return *m_pint; }

      string_t extract_str() { return std::move(*m_pstr); }

      null_t extract_null() { return std::move(*m_pnull); }

      object_t extract_object() { return std::move(*m_pobj); }

      ~Core() {}

      void delete_str() { m_pstr.~unique_ptr(); }

      void delete_null() { m_pnull.~unique_ptr(); }

      void delete_object() { m_pobj.~unique_ptr(); }

      void delete_integer() { m_pint.~unique_ptr(); }

    private:
      template <typename T> std::unique_ptr<T> Default() {
        return std::make_unique<T>();
      }

      template <typename T, typename... Ts>
      std::unique_ptr<T> make(Ts &&... args) {
        return std::make_unique<T>(std::forward<Ts>(args)...);
      }

      template <typename T, typename... Ts>
      void constructInner(std::unique_ptr<T> *inner, Ts &&... args) {
        new (inner) std::unique_ptr<T>{make<T>(std::forward<Ts>(args)...)};
      }

    }; // Core
  private:
    Type m_type{Type::null};
    Core m_core{};

  public:
    CoreWrapper() : m_type{Type::null}, m_core{} {}

    CoreWrapper(const char *str) : m_type{Type::string}, m_core{str} {}

    CoreWrapper(string_t &str) : m_type{Type::string}, m_core{str} {}

    CoreWrapper(string_t &&str)
        : m_type{Type::string}, m_core{std::forward<string_t>(str)} {}

    CoreWrapper(const object_t &obj) : m_type{Type::object}, m_core{obj} {}

    CoreWrapper(object_t &&obj)
        : m_type{Type::object}, m_core{std::forward<object_t>(obj)} {}

    CoreWrapper(int_t n) : m_type{Type::integer}, m_core{n} {}

    CoreWrapper(const CoreWrapper &other) : m_type{other.m_type} {
      switch (other.m_type) {
      case Type::string: {
        string_t strcopy = other.str();
        m_core = std::move(strcopy);
        break;
      }
      case Type::null: {
        null_t nullcopy = other.null();
        m_core = std::move(nullcopy);
        break;
      }
      case Type::object: {
        object_t objcopy = other.object();
        m_core = std::move(objcopy);
        break;
      }
      case Type::integer:
        m_core = other.integer();
        break;
      default:
        break;
      }
    }

    CoreWrapper(CoreWrapper &&other) noexcept : m_type{other.m_type} {
      switch (other.m_type) {
      case Type::string:
        m_core = other.extract_str();
        break;
      case Type::null:
        m_core = other.extract_null();
        break;
      case Type::object:
        m_core = other.extract_object();
        break;
      case Type::integer:
        m_core = other.integer();
        break;
      default:
        break;
      }
    }

    CoreWrapper &operator=(const CoreWrapper &other) {
      deleteCoreInner();
      m_type = other.m_type;

      switch (other.m_type) {
      case Type::string: {
        string_t strcopy = other.str();
        m_core = std::move(strcopy);
        break;
      }
      case Type::null: {
        null_t nullcopy = other.null();
        m_core = std::move(nullcopy);
        break;
      }
      case Type::object: {
        object_t objcopy = other.object();
        m_core = std::move(objcopy);
        break;
      }
      case Type::integer:
        m_core = other.integer();
        break;
      default:
        break;
      }
      return *this;
    }

    CoreWrapper &operator=(CoreWrapper &&other) {
      deleteCoreInner();
      m_type = other.m_type;

      switch (other.m_type) {
      case Type::string:
        m_core = other.extract_str();
        break;
      case Type::null:
        m_core = other.extract_null();
        break;
      case Type::object:
        m_core = other.extract_object();
        break;
      case Type::integer:
        m_core = other.integer();
        break;
      default:
        break;
      }
      return *this;
    }
    Type type() const { return m_type; }

    string_t &str() const { return m_core.str(); }

    null_t &null() const { return m_core.null(); }

    object_t &object() const { return m_core.object(); }

    int_t integer() const { return m_core.integer(); }

    string_t extract_str() {
      return std::forward<string_t>(m_core.extract_str());
    }

    null_t extract_null() {
      return std::forward<null_t>(m_core.extract_null());
    }

    object_t extract_object() {
      return std::forward<object_t>(m_core.extract_object());
    }

  private:
    void deleteCoreInner() {
      switch (m_type) {
      case Type::string:
        m_core.delete_str();
        break;
      case Type::null:
        m_core.delete_null();
        break;
      case Type::object:
        m_core.delete_object();
      case Type::integer:
        m_core.delete_integer();
        break;
      default:
        break;
      }
    }
  }; // CoreWrapper
private:
  CoreWrapper m_variant{};
}; // Json

} // namespace json

#endif

#include "jsoncommon.h"
#include "jsonobject.h"
#include "jsonstring.h"
