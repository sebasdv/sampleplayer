#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * SamplerPlayer - 16 Pad Sampler with MIDI support and waveform visualization
 */
class MainComponent : public juce::Component,
                      public juce::MidiInputCallback,
                      private juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Sampler Pad Button
    class SamplerPad : public juce::TextButton
    {
    public:
        SamplerPad (int padNumber);
        void paint (juce::Graphics& g) override;
        void setPlaying (bool isPlaying);
        bool isPlaying() const { return playing; }
        void setLoaded (bool isLoaded);

    private:
        int padNum;
        bool playing = false;
        bool loaded = false;
        juce::Colour padColour;
    };

    //==============================================================================
    void loadSampleFromFile (int padNumber, const juce::File& file);
    void playSample (int padNumber);
    void stopSample (int padNumber);
    void padButtonClicked (int padNumber);

    // MIDI callbacks
    void handleIncomingMidiMessage (juce::MidiInput* source, const juce::MidiMessage& message) override;
    void timerCallback() override;

    // Audio setup
    void prepareToPlay();
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    //==============================================================================
    // UI Components
    juce::TextButton loadFilesButton;
    juce::Label statusLabel;
    juce::OwnedArray<SamplerPad> pads;

    // Audio components
    juce::AudioDeviceManager audioDeviceManager;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSources[16];
    std::unique_ptr<juce::AudioTransportSource> transportSources[16];
    juce::MixerAudioSource mixerSource;
    juce::AudioSourcePlayer audioSourcePlayer;

    // Audio buffers for samples
    std::unique_ptr<juce::AudioBuffer<float>> sampleBuffers[16];
    bool sampleLoaded[16] = { false };

    // MIDI
    int lastInputIndex = 0;
    bool isAddingFromMidiInput = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
