#ifndef JSON_JSON_H
#define JSON_JSON_H

#include "null.h"

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
};

class Json {
public:
  using null_t = Null;
  using string_t = std::string;
  using object_t = std::map<string_t, Json>;

public: // Constructors
  Json() : m_variant{} {}

  Json(const char *str) : m_variant{str} {}

  Json(string_t &str) : m_variant{str} {}

  Json(string_t &&str) : m_variant{std::forward<string_t>(str)} {}

  Json(std::initializer_list<object_t::value_type> pairs)
      : m_variant{object_t{pairs}} {}

public: // Json common operations
  Type type() const { return m_variant.type(); }

  friend std::ostream &operator<<(std::ostream &out, const Json &jsn) {
    switch (jsn.m_variant.type()) {
    case Type::string:
      out << jsn.m_variant.str();
      break;
    case Type::null:
      out << jsn.m_variant.null();
      break;
    case Type::object:
      out << jsn.m_variant.object();
      break;
    default:
      out << "Not Implemented";
    }
    return out;
  }

public: // Json string operations
  std::vector<string_t::value_type> chars() const;
  string_t &&moveString();
  string_t copyString() const;
  bool isString() const;

public: // Json object operations
  std::vector<object_t::key_type> keys() const;
  std::vector<object_t::mapped_type> values() const;
  std::vector<object_t::value_type> items() const;
  object_t &&moveObject();
  object_t copyObject() const;
  friend std::ostream &operator<<(std::ostream &, const object_t &);
  friend std::ostream &operator<<(std::ostream &, const object_t::value_type &);
  bool isObject() const;
  const object_t::mapped_type &operator[](const object_t::key_type &) const;

private: // Json object private operations
  void assert_object_type() const;

private:
  struct CoreWrapper {
  private:
    union Core {
      std::unique_ptr<null_t> m_pnull;
      std::unique_ptr<string_t> m_pstr;
      std::unique_ptr<object_t> m_pobj;

      Core() : m_pnull{Default<null_t>()} {}

      Core(const char *str) : m_pstr{make<string_t>(str)} {}

      Core(string_t str) : m_pstr{make<string_t>(std::move(str))} {}

      Core(object_t obj) : m_pobj{make<object_t>(std::move(obj))} {}

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

      string_t &str() const { return *m_pstr; }

      null_t &null() const { return *m_pnull; }

      object_t &object() const { return *m_pobj; }

      string_t &&extract_str() { return std::move(*m_pstr); }

      null_t &&extract_null() { return std::move(*m_pnull); }

      object_t &&extract_object() { return std::move(*m_pobj); }

      ~Core() {}

      void delete_str() { m_pstr.~unique_ptr(); }

      void delete_null() { m_pnull.~unique_ptr(); }

      void delete_object() { m_pobj.~unique_ptr(); }

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

    CoreWrapper(object_t &obj) : m_type{Type::object}, m_core{obj} {}

    CoreWrapper(object_t &&obj)
        : m_type{Type::object}, m_core{std::forward<object_t>(obj)} {}

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
      default:
        break;
      }
    }

    CoreWrapper &operator=(const CoreWrapper &other) {
      if (m_type != other.m_type) {
        throw std::runtime_error{"TypeError: Assignment to different type"};
      }
      deleteCoreInner();
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
      default:
        break;
      }
      return *this;
    }

    CoreWrapper &operator=(CoreWrapper &&other) {
      if (m_type != other.m_type) {
        throw std::runtime_error{"TypeError: Assignment to different type"};
      }
      deleteCoreInner();
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
      default:
        break;
      }
      return *this;
    }
    Type type() const { return m_type; }

    string_t &str() const { return m_core.str(); }

    null_t &null() const { return m_core.null(); }

    object_t &object() const { return m_core.object(); }

    string_t &&extract_str() { return m_core.extract_str(); }

    null_t &&extract_null() { return m_core.extract_null(); }

    object_t &&extract_object() { return m_core.extract_object(); }

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

#include "jsonobject.h"
#include "jsonstring.h"
