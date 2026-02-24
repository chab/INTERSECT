#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <array>
#include <memory>
#include <vector>

class SampleData
{
public:
    struct PeakMipmap
    {
        int samplesPerPeak = 0;
        std::vector<float> maxPeaks;
        std::vector<float> minPeaks;
    };

    static constexpr int kNumMipmapLevels = 3;

    struct DecodedSample
    {
        juce::AudioBuffer<float> buffer;  // always stereo
        std::array<PeakMipmap, kNumMipmapLevels> peakMipmaps;
        juce::String fileName;
        juce::String filePath;
    };

    SampleData();

    static std::unique_ptr<DecodedSample> decodeFromFile (const juce::File& file,
                                                           double projectSampleRate);
    void applyDecodedSample (std::unique_ptr<DecodedSample> decoded);
    bool loadFromFile (const juce::File& file, double projectSampleRate);

    float getInterpolatedSample (double pos, int channel) const;

    int getNumFrames() const { return buffer.getNumSamples(); }
    bool isLoaded() const { return loaded; }

    const juce::AudioBuffer<float>& getBuffer() const { return buffer; }
    const std::array<PeakMipmap, kNumMipmapLevels>& getMipmaps() const { return peakMipmaps; }

    const juce::String& getFileName() const { return loadedFileName; }
    void setFileName (const juce::String& name) { loadedFileName = name; }

    const juce::String& getFilePath() const { return loadedFilePath; }
    void setFilePath (const juce::String& path) { loadedFilePath = path; }

private:
    void buildMipmaps();

    juce::AudioBuffer<float> buffer;  // always stereo
    std::array<PeakMipmap, kNumMipmapLevels> peakMipmaps;
    juce::String loadedFileName;
    juce::String loadedFilePath;
    bool loaded = false;
};
