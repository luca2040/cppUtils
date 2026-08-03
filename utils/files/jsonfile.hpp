#pragma once

// using json lib from https://github.com/nlohmann/json

#include <nlohmann/json.hpp>
#include <optional>

namespace utils::files
{

std::optional<nlohmann::json> readFileToJson(const std::filesystem::path& filepath);
bool writeJsonToFile(const std::filesystem::path& filepath, const nlohmann::json& jason);

} // namespace utils::files
