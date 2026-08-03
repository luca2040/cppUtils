#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include <vector>

namespace utils
{

template <typename StringType>
std::optional<std::vector<StringType>> parseStringList(const std::string& body)
{
    if (body.empty())
        return std::nullopt;

    try
    {
        auto json = nlohmann::json::parse(body);

        if (!json.is_array())
            return std::nullopt;

        std::vector<StringType> result;
        result.reserve(json.size());

        for (const auto& item : json)
        {
            if (!item.is_string())
                return std::nullopt;

            result.push_back(item.get<std::string>());
        }

        return result;
    }
    catch (const nlohmann::json::parse_error&)
    {
        return std::nullopt;
    }
}

} // namespace utils
