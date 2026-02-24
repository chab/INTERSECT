#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "WaveformCache.h"

class IntersectProcessor;

class WaveformView : public juce::Component,
                     public juce::FileDragAndDropTarget
{
public:
    explicit WaveformView (IntersectProcessor& p);

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;
    void mouseUp (const juce::MouseEvent& e) override;
    void mouseMove (const juce::MouseEvent& e) override;
    void mouseEnter (const juce::MouseEvent& e) override;
    void mouseExit (const juce::MouseEvent& e) override;
    void mouseWheelMove (const juce::MouseEvent& e, const juce::MouseWheelDetails& w) override;
    void modifierKeysChanged (const juce::ModifierKeys& mods) override;

    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;

    void rebuildCacheIfNeeded();

    bool sliceDrawMode = false;
    bool altModeActive = false;
    bool shiftPreviewActive = false;
    std::vector<int> transientPreviewPositions;

private:
    enum DragMode { None, DragEdgeLeft, DragEdgeRight, DrawSlice, MoveSlice, DuplicateSlice };

    enum class HoveredEdge { None, Left, Right };
    HoveredEdge hoveredEdge = HoveredEdge::None;

    int pixelToSample (int px) const;
    int sampleToPixel (int sample) const;

    void drawWaveform (juce::Graphics& g);
    void drawSlices (juce::Graphics& g);
    void drawPlaybackCursors (juce::Graphics& g);

    IntersectProcessor& processor;
    WaveformCache cache;
    float prevZoom = -1.0f, prevScroll = -1.0f;
    int prevWidth = 0;
    int prevNumFrames = 0;

    DragMode dragMode = None;
    int dragSliceIdx = -1;
    int drawStart = 0;
    int drawEnd = 0;
    int dragOffset = 0;    // for MoveSlice: offset from mouse to slice start
    int dragSliceLen = 0;  // for MoveSlice: original slice length
    int dragPreviewStart = 0; // for edge/move drags: preview start sample
    int dragPreviewEnd = 0;   // for edge/move drags: preview end sample
    int ghostStart = 0;    // for DuplicateSlice: ghost overlay start sample
    int ghostEnd   = 0;    // for DuplicateSlice: ghost overlay end sample

    // Middle-mouse drag (scroll+zoom like ScrollZoomBar)
    bool midDragging = false;
    float midDragStartZoom = 1.0f;
    float midDragAnchorFrac = 0.0f;
    float midDragAnchorPixelFrac = 0.0f;
    int   midDragStartX = 0;
    int   midDragStartY = 0;
};
