# nlohmann/json.hpp
# =================
# Placeholder for nlohmann/json library
#
# To use JSON in this project, download the single-header version from:
# https://github.com/nlohmann/json/releases
#
# Or install via: sudo apt-get install nlohmann-json3-dev
#
# This file is intentionally left minimal to allow compilation without
# the full JSON library. Replace with the full header if JSON support is needed.

#ifndef NLOHMANN_JSON_HPP
#define NLOHMANN_JSON_HPP

#include <string>
#include <map>
#include <vector>
#include <stdexcept>

/**
 * @brief Minimal JSON class placeholder.
 *
 * This is a stub implementation. For full functionality,
 * replace with nlohmann/json library.
 */
namespace nlohmann {

class json {
public:
    json() = default;
    ~json() = default;

    // Basic type support
    json(const std::string& s) {}
    json(int i) {}
    json(double d) {}
    json(bool b) {}

    // Serialization
    std::string dump(int indent = -1) const { return "{}"; }

    // Parsing (stub)
    static json parse(const std::string& s) { return json(); }

    // Access
    json& operator[](const std::string& key) { return *this; }
    json& operator[](size_t index) { return *this; }

    // Type checking
    bool is_null() const { return true; }
    bool is_string() const { return false; }
    bool is_number() const { return false; }
    bool is_boolean() const { return false; }
    bool is_array() const { return false; }
    bool is_object() const { return false; }
};

} // namespace nlohmann

#endif // NLOHMANN_JSON_HPP
