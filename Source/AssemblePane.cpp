#include "AssemblePane.h"

#include <cstdlib>
#include <memory>
#include <string>

#include "WaveDisplay.h"
#include "juce_core/system/juce_PlatformDefs.h"
#include "juce_gui_basics/juce_gui_basics.h"

AssemblePane::AssemblePane(AudioPlayer* _player, juce::AudioFormatManager& _formatManagerToUse,
                           juce::AudioThumbnailCache& _cacheToUse)
    : player(_player), waveDisplay(_formatManagerToUse, _cacheToUse), liveAudioVisualiser(new LiveAudioVisualiser()) {
        // Set up buttons and initialize look and feel for the sliders
        setupButton(&playButton);
        setupButton(&stopButton);
        setupButton(&loadButton);

        // Configure different slider styles and colors for visual consistency
        otherLookAndFeel1.setColour(juce::Slider::thumbColourId, juce::Colours::orangered);
        otherLookAndFeel2.setColour(juce::Slider::thumbColourId, juce::Colours::forestgreen);
        otherLookAndFeel3.setColour(juce::Slider::thumbColourId, juce::Colours::cornflowerblue);

        // Add sliders for volume, position, speed, damping, and others to the component
        addAndMakeVisible(speedSlider);
        addAndMakeVisible(positionSlider);

        // Set up volume slider with defined range and style
        SliderParams volSliderParams = {
            .range = {0.0, 1.0},
            .defaultValue = 0.5,
            .numDecimalPlaces = 2,
            .style = juce::Slider::LinearVertical,
        };
        setupSlider(&volSlider, &otherLookAndFeel1, volSliderParams);
        setupLabel(&volSlider, &volLabel, "Volume");

        // Set up speed slider with defined range and style
        SliderParams speedSliderParams = {
            .range = {0.1, 2.0},
            .defaultValue = 1.0,
            .numDecimalPlaces = 2,
            .style = juce::Slider::Rotary,
        };
        setupSlider(&speedSlider, &otherLookAndFeel2, speedSliderParams);
        setupLabel(&speedSlider, &speedLabel, "Speed");

        // Set up damping slider with defined range and style
        SliderParams dampingSliderParams = {
            .range = {0.0, 1.0},
            .defaultValue = 0.0,
            .numDecimalPlaces = 2,
            .style = juce::Slider::LinearHorizontal,
            .textValueSuffix = new std::string(" factor"),
        };
        setupSlider(&dampingSlider, &otherLookAndFeel3, dampingSliderParams);
        setupLabel(&dampingSlider, &dampingLabel, "Damping");

        // Set up position slider with defined range and style
        SliderParams positionSliderParams = {
            .range = {0.0, 100.0},
            .defaultValue = 10.0,
            .numDecimalPlaces = 2,
            .style = juce::Slider::LinearHorizontal,
            .textValueSuffix = new std::string(" % of track"),
        };
        setupSlider(&positionSlider, &otherLookAndFeel3, positionSliderParams);
        setupLabel(&positionSlider, &positionLabel, "Position");

        // Set up additional knobs for bass, mid, and tremble adjustments
        SliderParams bassKnobParam = {
            .range = {-24.0, 24.0},
            .defaultValue = 0.0,
            .numDecimalPlaces = 1,
            .style = juce::Slider::Rotary,
            .textValueSuffix = new std::string("Bass"),
        };

        SliderParams midKnobParam = {
            .range = {-24.0, 24.0},
            .defaultValue = 0.0,
            .numDecimalPlaces = 1,
            .style = juce::Slider::Rotary,
            .textValueSuffix = new std::string("Mid"),
        };

        SliderParams trembleKnobParam = {
            .range = {-24.0, 24.0},
            .defaultValue = 0.0,
            .numDecimalPlaces = 1,
            .style = juce::Slider::Rotary,
            .textValueSuffix = new std::string("Tremble"),
        };

        setupSlider(&bassSlider,  &otherLookAndFeel2, bassKnobParam);
        setupSlider(&midSlider, &otherLookAndFeel1, midKnobParam);
        setupSlider(&trembleSlider, &otherLookAndFeel1, trembleKnobParam);

        // Set up frequency slider with defined range and style
        SliderParams freqSliderParams = {
            .range = {20.0, 20000.0},
            .defaultValue = 20.0,
            .numDecimalPlaces = 0,
            .style = juce::Slider::Rotary,
            .textValueSuffix = new std::string(" Hz"),
        };
        setupSlider(&freqSlider, &otherLookAndFeel1, freqSliderParams);

        // Add wave display and visualizer to the component
        addAndMakeVisible(waveDisplay);

        // Initialize the live audio visualizer and configure its settings
        if (liveAudioVisualiser == nullptr) {
                DBG("AssemblePane.cpp::liveAudioVisualiser is null");
        }

        liveAudioVisualiser->setBufferSize(1024);
        liveAudioVisualiser->setSamplesPerBlock(16);

        addAndMakeVisible(*liveAudioVisualiser);
        player->setPlayerVisualiser(liveAudioVisualiser);

        // Start a timer for periodic updates (e.g., position tracking)
        startTimer(500);
}

AssemblePane::~AssemblePane() {};

/**
 * Sets up a button, making it visible and adding it to the component's event listener.
 */
void AssemblePane::setupButton(juce::Button* component) {
        addAndMakeVisible(component);
        component->addListener(this);
};

/**
 * Configures a slider with specified parameters, look and feel, and style.
 */
void AssemblePane::setupSlider(juce::Slider* component, LookAndFeel* lookAndFeel, SliderParams params) {
        addAndMakeVisible(component);
        component->addListener(this);
        component->setLookAndFeel(lookAndFeel);

        component->setSliderStyle(params.style);
        component->setRange(std::get<0>(params.range), std::get<1>(params.range));
        component->setValue(params.defaultValue);
        component->setNumDecimalPlacesToDisplay(params.numDecimalPlaces);

        component->setTextBoxStyle(juce::Slider::TextBoxLeft, false, component->getTextBoxWidth(),
                                   component->getTextBoxHeight());

        // Set suffix if provided for slider text value
        if (params.textValueSuffix != nullptr) {
                component->setTextValueSuffix(*params.textValueSuffix);
                DBG(*params.textValueSuffix);
        }
};

/**
 * Attaches a label to a component and makes the label visible.
 */
void AssemblePane::setupLabel(juce::Component* target, juce::Label* label, std::string text) {
        addAndMakeVisible(label);
        label->setText(text, juce::dontSendNotification);
        label->attachToComponent(target, true);
};

/**
 * Paints the background and visual elements on the component.
 * @param g The graphics context used to render the component.
 */
void AssemblePane::paint(juce::Graphics& g) {
        g.fillAll(juce::Colour{122, 95, 138});

        g.setColour(juce::Colours::grey);
        g.drawRect(getLocalBounds(), 1);

        g.setColour(juce::Colours::navajowhite);
        g.setFont(30.0f);
        g.drawText("DJ Audio", 10, 10, getWidth(), 10, juce::Justification::centred, true);
}

/**
 * Resizes and arranges child components within the AssemblePane.
 */
void AssemblePane::resized() {
        int rowHeight = getHeight() / 10;
        double width = getWidth() - 20;
        int sliderLeftMargin = 60;

        double thirdWidth = width / 3;

        // Position buttons
        playButton.setBounds(15, 0, thirdWidth - 10, rowHeight);
        stopButton.setBounds(thirdWidth + 15, 0, thirdWidth - 10, rowHeight);
        loadButton.setBounds(2 * thirdWidth + 15, 0, thirdWidth - 10, rowHeight);

        // Position rotary sliders
        volSlider.setBounds(sliderLeftMargin, 10 + playButton.getBounds().getBottom(), width / 2 - sliderLeftMargin,
                            rowHeight * 2);
        speedSlider.setBounds(sliderLeftMargin + width / 2, 10 + playButton.getBounds().getBottom(),
                              width / 2 - sliderLeftMargin, rowHeight * 2);

        // Position horizontal sliders
        positionSlider.setBounds(sliderLeftMargin, 10 + speedSlider.getBounds().getBottom(), width - sliderLeftMargin,
                                 rowHeight);
        dampingSlider.setBounds(sliderLeftMargin, 10 + positionSlider.getBounds().getBottom(), width - sliderLeftMargin,
                                rowHeight);

        // Position bass, mid, and tremble sliders
        bassSlider.setBounds(sliderLeftMargin, 10 + dampingSlider.getBounds().getBottom(), width / 3 - sliderLeftMargin,
                             rowHeight * 2);
        midSlider.setBounds(bassSlider.getBounds().getRight(), 10 + dampingSlider.getBounds().getBottom(),
                            width / 3 - sliderLeftMargin, rowHeight * 2);
        trembleSlider.setBounds(midSlider.getBounds().getRight(), 10 + dampingSlider.getBounds().getBottom(),
                                width / 3 - sliderLeftMargin, rowHeight * 2);

        // Position wave display and visualizer
        waveDisplay.setBounds(5, trembleSlider.getBounds().getBottom(), width - 10, rowHeight * 2);
        liveAudioVisualiser->setBounds(5, waveDisplay.getBounds().getBottom(), width - 10, rowHeight);
}

/**
 * Handles button click events for play, stop, and load buttons.
 */
void AssemblePane::buttonClicked(juce::Button* button) {
        if (button == &playButton) {
                std::cout << "Play button clicked" << std::endl;
                int secs = player->getLengthInSeconds();
                DBG("Length of track: " << secs);

                // Start playback from the player
                player->start();
        }
        if (button == &stopButton) {
                player->stop();
        }
        if (button == &loadButton) {
                constexpr auto folderChooserFlags = FileBrowserComponent::canSelectFiles |
                                                    FileBrowserComponent::openMode;

                fChooser.launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser) {
                        const juce::File file(chooser.getResult());
                        DBG(file.getFullPathName());
                        loadFile(juce::URL{file});
                });
        }
}

/**
 * Handles slider value change events for adjusting various audio parameters.
 */
void AssemblePane::sliderValueChanged(juce::Slider* slider) {
        double value = slider->getValue();

        if (slider == &volSlider) {
                std::cout << value << std::endl;
                player->setGain(value);
        }

        if (slider == &speedSlider) {
                std::cout << "Speed Slider changed" << value << std::endl;
                player->setSpeed(value);
        }

        if (slider == &positionSlider) {
                std::cout << "Position Slider changed" << value << std::endl;
                if (abs(value - player->getPositionRelative()) > 2) {
                        player->setPositionRelative(value / 100);
                }
        }
        if (slider == &dampingSlider) {
                std::cout << "Damping Slider changed" << value << std::endl;
                player->setDamping(value);
        }

        if (slider == &bassSlider) {
                std::cout << "Bass Slider changed" << value << std::endl;
                player->setBassGain(value);
        }

        if (slider == &trembleSlider) {
                std::cout << "Tremble Slider changed" << value << std::endl;
                player->setTrebleGain(value);
        }
}

/**
 * Handles file drag events (currently not implemented).
 */
bool AssemblePane::isInterestedInFileDrag(const juce::StringArray& files) { return false; }

/**
 * Handles file drop events to load a dropped audio file.
 */
void AssemblePane::filesDropped(const juce::StringArray& files, int, int y) {
        DBG("Files dropped");
        if (files.size() == 1) {
                loadFile(juce::URL{files[0]});
        }
}

/**
 * Periodically updates the position of the sliders and visualizer based on the current playback state.
 */
void AssemblePane::timerCallback() {
        positionSlider.setValue(player->getPositionRelative());
        waveDisplay.setPositionRelative(player->getPositionRelative());
}

/**
 * Loads an audio file from the specified URL and updates visual elements accordingly.
 */
void AssemblePane::loadFile(juce::URL audioURL) {
        DBG("AssemblePane::loadFile called");
        DBG(audioURL.toString(true));
        player->loadUrl(audioURL);
        waveDisplay.loadURL(audioURL);
}
