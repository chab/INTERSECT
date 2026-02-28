#include "ActionPanel.h"
#include "AutoChopPanel.h"
#include "IntersectLookAndFeel.h"
#include "WaveformView.h"
#include "../PluginProcessor.h"

ActionPanel::ActionPanel (IntersectProcessor& p, WaveformView& wv)
    : processor (p), waveformView (wv)
{
    addAndMakeVisible (addSliceBtn);
    addAndMakeVisible (lazyChopBtn);
    addAndMakeVisible (dupBtn);
    addAndMakeVisible (splitBtn);
    addAndMakeVisible (deleteBtn);
    addAndMakeVisible (snapBtn);
    addAndMakeVisible (midiSelectBtn);

    for (auto* btn : { &addSliceBtn, &lazyChopBtn, &dupBtn, &splitBtn, &deleteBtn,
                       &snapBtn, &midiSelectBtn })
    {
        btn->setColour (juce::TextButton::buttonColourId, getTheme().button);
        btn->setColour (juce::TextButton::textColourOnId, getTheme().foreground);
        btn->setColour (juce::TextButton::textColourOffId, getTheme().foreground);
    }

    addSliceBtn.onClick = [this] { triggerAddSliceMode(); };
    lazyChopBtn.onClick = [this] { triggerLazyChop(); };
    dupBtn.onClick = [this] { triggerDuplicateSlice(); };
    splitBtn.onClick = [this] { triggerAutoChop(); };

    addSliceBtn.setTooltip ("Add Slice (Shift+A / hold Alt)");
    lazyChopBtn.setTooltip ("Lazy Chop (Shift+Z)");
    dupBtn.setTooltip ("Duplicate Slice (Shift+D)");
    splitBtn.setTooltip ("Auto Chop (Shift+C)");
    deleteBtn.setTooltip ("Delete Slice (Del)");

    snapBtn.setTooltip ("Snap to Zero-Crossing (Shift+X)");
    snapBtn.onClick = [this] { toggleSnapToZeroCrossing(); };
    updateSnapButtonAppearance (false);

    deleteBtn.onClick = [this] { triggerDeleteSelectedSlice(); };

    midiSelectBtn.setTooltip ("Follow MIDI (Shift+F)");
    midiSelectBtn.onClick = [this] { toggleFollowMidiSelection(); };
    updateMidiButtonAppearance (false);
}

ActionPanel::~ActionPanel() = default;

void ActionPanel::triggerAddSliceMode()
{
    const bool nextState = ! waveformView.isSliceDrawModeActive();
    waveformView.setSliceDrawMode (nextState);
    if (nextState)
        waveformView.showOverlayHint ("ADD mode: drag on waveform to create a slice.", 0, true);
    else
        waveformView.clearOverlayHint();
    repaint();
}

void ActionPanel::triggerLazyChop()
{
    IntersectProcessor::Command cmd;
    if (processor.lazyChop.isActive())
        cmd.type = IntersectProcessor::CmdLazyChopStop;
    else
        cmd.type = IntersectProcessor::CmdLazyChopStart;
    processor.pushCommand (cmd);
    repaint();
}

void ActionPanel::triggerDuplicateSlice()
{
    IntersectProcessor::Command cmd;
    cmd.type = IntersectProcessor::CmdDuplicateSlice;
    cmd.intParam1 = -1;
    processor.pushCommand (cmd);
    repaint();
}

void ActionPanel::triggerAutoChop()
{
    if (autoChopPanel != nullptr)
    {
        toggleAutoChop();
        return;
    }

    const auto& ui = processor.getUiSliceSnapshot();
    const int sel = ui.selectedSlice;
    if (sel < 0 || sel >= ui.numSlices)
    {
        waveformView.showOverlayHint ("Select a slice first, then press AUTO.", 2200);
        return;
    }

    toggleAutoChop();
}

void ActionPanel::triggerDeleteSelectedSlice()
{
    const auto& ui = processor.getUiSliceSnapshot();
    const int sel = ui.selectedSlice;
    if (sel < 0)
        return;

    IntersectProcessor::Command cmd;
    cmd.type = IntersectProcessor::CmdDeleteSlice;
    cmd.intParam1 = sel;
    processor.pushCommand (cmd);
}

void ActionPanel::toggleSnapToZeroCrossing()
{
    const bool newState = ! processor.snapToZeroCrossing.load();
    processor.snapToZeroCrossing.store (newState);
    updateSnapButtonAppearance (newState);
    repaint();
}

void ActionPanel::toggleFollowMidiSelection()
{
    const bool newState = ! processor.midiSelectsSlice.load();
    processor.midiSelectsSlice.store (newState);
    updateMidiButtonAppearance (newState);
    repaint();
}

void ActionPanel::toggleAutoChop()
{
    if (autoChopPanel != nullptr)
    {
        if (auto* parent = autoChopPanel->getParentComponent())
            parent->removeChildComponent (autoChopPanel.get());
        autoChopPanel.reset();
        return;
    }

    autoChopPanel = std::make_unique<AutoChopPanel> (processor, waveformView);

    if (auto* editor = waveformView.getParentComponent())
    {
        auto wfBounds = waveformView.getBoundsInParent();
        int panelH = 34;
        int panelX = wfBounds.getX();
        int panelW = wfBounds.getWidth();
        int panelY = wfBounds.getBottom() - panelH;
        autoChopPanel->setBounds (panelX, panelY, panelW, panelH);
        editor->addAndMakeVisible (*autoChopPanel);
    }
}

void ActionPanel::resized()
{
    const int gap       = 6;
    const int btnH      = getHeight();
    const int snapW     = 30;
    const int midiW     = 30;
    const int utilityTotal = snapW + midiW + gap;
    const int availW    = getWidth() - utilityTotal - gap;
    const int numBtns   = 5;
    const int totalGap  = gap * (numBtns - 1);
    const int btnW      = (availW - totalGap) / numBtns;

    addSliceBtn.setBounds (0,                0, btnW, btnH);
    lazyChopBtn.setBounds (btnW + gap,       0, btnW, btnH);
    splitBtn.setBounds    (2 * (btnW + gap), 0, btnW, btnH);
    dupBtn.setBounds      (3 * (btnW + gap), 0, btnW, btnH);
    deleteBtn.setBounds   (4 * (btnW + gap), 0, btnW, btnH);

    midiSelectBtn.setBounds (getWidth() - midiW,            0, midiW, btnH);
    snapBtn.setBounds       (getWidth() - midiW - gap - snapW, 0, snapW, btnH);
}

void ActionPanel::paint (juce::Graphics& g)
{
    for (auto* btn : { &addSliceBtn, &lazyChopBtn, &dupBtn, &splitBtn, &deleteBtn })
    {
        btn->setColour (juce::TextButton::buttonColourId, getTheme().button);
        btn->setColour (juce::TextButton::textColourOnId, getTheme().foreground);
        btn->setColour (juce::TextButton::textColourOffId, getTheme().foreground);
    }

    updateMidiButtonAppearance (processor.midiSelectsSlice.load());
    updateSnapButtonAppearance (processor.snapToZeroCrossing.load());

    if (waveformView.isSliceDrawModeActive())
    {
        g.setColour (getTheme().accent.withAlpha (0.25f));
        g.fillRect (addSliceBtn.getBounds());
    }

    if (processor.lazyChop.isActive())
    {
        lazyChopBtn.setButtonText ("STOP");
        g.setColour (juce::Colours::red.withAlpha (0.25f));
        g.fillRect (lazyChopBtn.getBounds());
    }
    else
    {
        lazyChopBtn.setButtonText ("LAZY");
    }
}

void ActionPanel::updateMidiButtonAppearance (bool active)
{
    if (active)
    {
        midiSelectBtn.setColour (juce::TextButton::textColourOnId,  getTheme().accent);
        midiSelectBtn.setColour (juce::TextButton::textColourOffId, getTheme().accent);
        midiSelectBtn.setColour (juce::TextButton::buttonColourId,  getTheme().accent.withAlpha (0.2f));
    }
    else
    {
        midiSelectBtn.setColour (juce::TextButton::textColourOnId,  getTheme().foreground);
        midiSelectBtn.setColour (juce::TextButton::textColourOffId, getTheme().foreground);
        midiSelectBtn.setColour (juce::TextButton::buttonColourId,  getTheme().button);
    }
}

void ActionPanel::updateSnapButtonAppearance (bool active)
{
    if (active)
    {
        snapBtn.setColour (juce::TextButton::textColourOnId,  getTheme().accent);
        snapBtn.setColour (juce::TextButton::textColourOffId, getTheme().accent);
        snapBtn.setColour (juce::TextButton::buttonColourId,  getTheme().accent.withAlpha (0.2f));
    }
    else
    {
        snapBtn.setColour (juce::TextButton::textColourOnId,  getTheme().foreground);
        snapBtn.setColour (juce::TextButton::textColourOffId, getTheme().foreground);
        snapBtn.setColour (juce::TextButton::buttonColourId,  getTheme().button);
    }
}
