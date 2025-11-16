#include "MainComponent.h"

//==============================================================================
// SamplerPad Implementation
MainComponent::SamplerPad::SamplerPad (int padNumber)
    : TextButton ("Pad " + juce::String (padNumber)),
      padNum (padNumber)
{
    // Color scheme matching the web version
    const juce::Colour colours[] = {
        juce::Colour (0xFFE6E8BF), // Pad 1, 5, 9, 13
        juce::Colour (0xFFDDDA00), // Pad 2, 6, 10, 14
        juce::Colour (0xFFF27900), // Pad 3, 7, 11, 15
        juce::Colour (0xFFDD1D00)  // Pad 4, 8, 12, 16
    };

    padColour = colours[(padNumber - 1) % 4];
}

void MainComponent::SamplerPad::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Neumorphic style shadow effect
    if (playing)
    {
        // Pressed/playing state - inset shadow
        g.setColour (padColour.darker (0.3f));
        g.fillRoundedRectangle (bounds, 20.0f);
    }
    else
    {
        // Normal state - outset shadow
        g.setColour (padColour);
        g.fillRoundedRectangle (bounds, 20.0f);

        // Add subtle shadow
        g.setColour (juce::Colours::black.withAlpha (0.1f));
        g.drawRoundedRectangle (bounds.reduced (2.0f), 20.0f, 2.0f);
    }

    // Draw pad number
    g.setColour (juce::Colours::black.withAlpha (0.7f));
    g.setFont (20.0f);
    g.drawText (juce::String (padNum), bounds, juce::Justification::centred);

    // Show loaded indicator
    if (loaded)
    {
        g.setColour (juce::Colours::green.withAlpha (0.5f));
        g.fillEllipse (bounds.getWidth() - 15, 5, 10, 10);
    }
}

void MainComponent::SamplerPad::setPlaying (bool isPlaying)
{
    playing = isPlaying;
    repaint();
}

void MainComponent::SamplerPad::setLoaded (bool isLoaded)
{
    loaded = isLoaded;
    repaint();
}

//==============================================================================
// MainComponent Implementation
MainComponent::MainComponent()
{
    // Setup audio formats
    formatManager.registerBasicFormats();

    // Setup audio device
    audioDeviceManager.initialiseWithDefaultDevices (0, 2);
    audioDeviceManager.addAudioCallback (&audioSourcePlayer);
    audioSourcePlayer.setSource (&mixerSource);

    // Initialize mixer with 16 inputs
    mixerSource.removeAllInputs();

    // Create load files button
    loadFilesButton.setButtonText ("Cargar Archivos");
    loadFilesButton.onClick = [this]
    {
        juce::FileChooser chooser ("Selecciona archivos de audio",
                                   juce::File::getSpecialLocation (juce::File::userHomeDirectory),
                                   "*.wav;*.mp3;*.aif;*.aiff;*.ogg;*.flac");

        if (chooser.browseForMultipleFilesToOpen())
        {
            auto files = chooser.getResults();
            for (int i = 0; i < files.size() && i < 16; ++i)
            {
                loadSampleFromFile (i + 1, files[i]);
            }
        }
    };
    addAndMakeVisible (loadFilesButton);

    // Create status label
    statusLabel.setText ("Sampler Player Listo", juce::dontSendNotification);
    statusLabel.setJustificationType (juce::Justification::centred);
    statusLabel.setColour (juce::Label::textColourId, juce::Colour (0xFFd47007));
    addAndMakeVisible (statusLabel);

    // Create 16 pads
    for (int i = 1; i <= 16; ++i)
    {
        auto* pad = pads.add (new SamplerPad (i));
        pad->onClick = [this, i] { padButtonClicked (i); };
        addAndMakeVisible (pad);

        // Initialize transport sources
        transportSources[i-1] = std::make_unique<juce::AudioTransportSource>();
        mixerSource.addInputSource (transportSources[i-1].get(), false);
    }

    // Setup MIDI
    auto midiInputs = juce::MidiInput::getAvailableDevices();
    if (!midiInputs.isEmpty())
    {
        audioDeviceManager.setMidiInputDeviceEnabled (midiInputs[0].identifier, true);
        audioDeviceManager.addMidiInputDeviceCallback (midiInputs[0].identifier, this);
        statusLabel.setText ("MIDI Conectado - Listo", juce::dontSendNotification);
    }

    // Set window size
    setSize (600, 800);

    // Start timer for animations
    startTimerHz (30);
}

MainComponent::~MainComponent()
{
    stopTimer();
    audioSourcePlayer.setSource (nullptr);
    audioDeviceManager.removeAudioCallback (&audioSourcePlayer);

    for (int i = 0; i < 16; ++i)
    {
        if (transportSources[i] != nullptr)
        {
            transportSources[i]->setSource (nullptr);
        }
    }
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFFd9d2c6)); // Background color matching web version
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced (20);

    // Title area
    auto titleArea = area.removeFromTop (50);

    // Button area
    auto buttonArea = area.removeFromTop (60);
    loadFilesButton.setBounds (buttonArea.withSizeKeepingCentre (200, 40));

    // Status area
    auto statusArea = area.removeFromTop (30);
    statusLabel.setBounds (statusArea);

    area.removeFromTop (10); // Gap

    // Pad grid (4x4)
    const int padding = 20;
    const int numRows = 4;
    const int numCols = 4;

    auto gridWidth = area.getWidth();
    auto gridHeight = area.getHeight();

    auto padSize = juce::jmin ((gridWidth - padding * (numCols - 1)) / numCols,
                               (gridHeight - padding * (numRows - 1)) / numRows);

    for (int row = 0; row < numRows; ++row)
    {
        for (int col = 0; col < numCols; ++col)
        {
            int padIndex = row * numCols + col;
            int x = area.getX() + col * (padSize + padding);
            int y = area.getY() + row * (padSize + padding);

            pads[padIndex]->setBounds (x, y, padSize, padSize);
        }
    }
}

void MainComponent::padButtonClicked (int padNumber)
{
    playSample (padNumber);
}

void MainComponent::loadSampleFromFile (int padNumber, const juce::File& file)
{
    std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor (file));

    if (reader != nullptr)
    {
        // Create buffer to hold the audio data
        sampleBuffers[padNumber - 1] = std::make_unique<juce::AudioBuffer<float>>(
            static_cast<int>(reader->numChannels),
            static_cast<int>(reader->lengthInSamples)
        );

        // Read the audio file into the buffer
        reader->read (sampleBuffers[padNumber - 1].get(),
                     0,
                     static_cast<int>(reader->lengthInSamples),
                     0,
                     true,
                     true);

        // Create a new reader for the source
        std::unique_ptr<juce::AudioFormatReader> readerForSource (formatManager.createReaderFor (file));

        // Create reader source
        readerSources[padNumber - 1] = std::make_unique<juce::AudioFormatReaderSource>(
            readerForSource.release(), true
        );

        sampleLoaded[padNumber - 1] = true;
        pads[padNumber - 1]->setLoaded (true);

        statusLabel.setText ("Archivo cargado en Pad " + juce::String (padNumber),
                           juce::dontSendNotification);
    }
}

void MainComponent::playSample (int padNumber)
{
    if (!sampleLoaded[padNumber - 1])
    {
        statusLabel.setText ("No hay sample en Pad " + juce::String (padNumber),
                           juce::dontSendNotification);
        return;
    }

    // Stop if already playing
    if (transportSources[padNumber - 1]->isPlaying())
    {
        transportSources[padNumber - 1]->stop();
    }

    // Set the source and play
    transportSources[padNumber - 1]->setSource (readerSources[padNumber - 1].get());
    transportSources[padNumber - 1]->setPosition (0);
    transportSources[padNumber - 1]->start();

    pads[padNumber - 1]->setPlaying (true);
    statusLabel.setText ("Reproduciendo Pad " + juce::String (padNumber),
                       juce::dontSendNotification);
}

void MainComponent::handleIncomingMidiMessage (juce::MidiInput* source,
                                               const juce::MidiMessage& message)
{
    if (message.isNoteOn())
    {
        int padNumber = (message.getNoteNumber() % 16) + 1;
        playSample (padNumber);
    }
}

void MainComponent::timerCallback()
{
    // Update playing status of pads
    for (int i = 0; i < 16; ++i)
    {
        if (transportSources[i] != nullptr)
        {
            bool isPlaying = transportSources[i]->isPlaying();
            if (pads[i]->isPlaying() != isPlaying)
            {
                pads[i]->setPlaying (isPlaying);
            }
        }
    }
}
