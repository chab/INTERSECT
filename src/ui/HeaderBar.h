#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class IntersectProcessor;

class HeaderBar : public juce::Component
{
public:
    explicit HeaderBar (IntersectProcessor& p);
    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;

private:
    void showThemePopup();
    void adjustScale (float delta);
    void openFileBrowser();
    void openRelinkBrowser();

    IntersectProcessor& processor;
    juce::TextButton undoBtn  { "UNDO" };
    juce::TextButton redoBtn  { "REDO" };
    juce::TextButton panicBtn { "PANIC" };
    juce::TextButton loadBtn  { "LOAD" };
    juce::TextButton themeBtn { "SET" };

    std::unique_ptr<juce::FileChooser> fileChooser;

    juce::Rectangle<int> sampleInfoBounds;
};
