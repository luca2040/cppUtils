#pragma once

#include <functional>
#include <imgui.h>
#include <optional>

namespace utils
{

template <typename ListType, typename ItemType>
void imguiChecklist(const ListType& itemsList, const std::optional<ItemType>& selectedItem,
                    const std::function<const char*(const ItemType&)>&         getNameFunc,
                    const std::function<void(const std::optional<ItemType>&)>& onChangeFunc)
{
    bool                    somethingChanged = false;
    std::optional<ItemType> newSelectedItem  = selectedItem;

    for (auto& item : itemsList)
    {
        bool isSelected, wasSelected;
        if (newSelectedItem.has_value() && newSelectedItem.value() == item)
            isSelected = wasSelected = true;
        else
            isSelected = wasSelected = false;

        ImGui::Checkbox(getNameFunc(item), &isSelected);

        if (isSelected)
            newSelectedItem = item;
        else if (wasSelected)
            newSelectedItem = std::nullopt;

        if (isSelected != wasSelected)
            somethingChanged = true;
    }
    if (somethingChanged)
        onChangeFunc(newSelectedItem);
}

} // namespace utils
