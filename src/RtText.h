#pragma once

#include <array>
#include <limits>

#include <juce_core/juce_core.h>

template <size_t Capacity>
struct RtText
{
    static_assert (Capacity > 1, "RtText capacity must allow room for a terminator");
    static_assert (Capacity <= (size_t) std::numeric_limits<int>::max(),
                   "RtText capacity must fit in int for JUCE UTF-8 helpers");
    static_assert (Capacity - 1 <= (size_t) std::numeric_limits<uint16_t>::max(),
                   "RtText capacity must fit in uint16_t length storage");

    static constexpr int kCapacityInt = static_cast<int> (Capacity);
    static constexpr size_t kMaxStoredLength = Capacity - 1;

    std::array<char, Capacity> bytes {};
    uint16_t length = 0;

    RtText()
    {
        clear();
    }

    void clear() noexcept
    {
        length = 0;
        bytes[0] = '\0';
    }

    bool isEmpty() const noexcept
    {
        return length == 0;
    }

    void assign (const juce::String& text) noexcept
    {
        clear();
        const auto written = text.copyToUTF8 (bytes.data(), kCapacityInt);
        if (written <= 0)
            return;

        const auto storedLength = juce::jmin (written - 1, kMaxStoredLength);
        length = static_cast<uint16_t> (storedLength);
        bytes[(size_t) length] = '\0';
    }

    juce::String toString() const
    {
        return juce::String::fromUTF8 (bytes.data(), (int) length);
    }
};
