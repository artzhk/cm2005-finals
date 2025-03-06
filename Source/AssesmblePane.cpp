
#include "AssesmblePane.h"

#include <string>

#include "WaveDisplay.h"
#include "juce_core/system/juce_PlatformDefs.h"
#include "juce_gui_basics/juce_gui_basics.h"

AssemblePane::AssemblePane(AudioPlayer* _player, juce::AudioFormatManager& _formatManagerToUse,
                           juce::AudioThumbnailCache& _cacheToUse)
    : player(_player), waveDisplay(_formatManagerToUse, _cacheToUse) {
        // In your constructor, you should add any child components, and initialise any special settings that your
        // component needs.

        // TODO: add high mid and low sliders controls

        setupButton(&playButton);
        setupButton(&stopButton);
        setupButton(&loadButton);

        otherLookAndFeel1.setColour(juce::Slider::thumbColourId, juce::Colours::orangered);
        otherLookAndFeel2.setColour(juce::Slider::thumbColourId, juce::Colours::forestgreen);
        otherLookAndFeel3.setColour(juce::Slider::thumbColourId, juce::Colours::cornflowerblue);

        addAndMakeVisible(speedSlider);
        addAndMakeVisible(positionSlider);

        SliderParams volSliderParams = {
            .range = {0.0, 1.0},
            .defaultValue = 0.5,
            .numDecimalPlaces = 2,
            .style = juce::Slider::LinearVertical,
        };

        setupSlider(&volSlider, &otherLookAndFeel1, volSliderParams);
        setupLabel(&volSlider, &volLabel, "Volume");

        SliderParams speedSliderParams = {
            .range = {0.1, 2.0},
            .defaultValue = 1.0,
            .numDecimalPlaces = 2,
            .style = juce::Slider::Rotary,
        };

        setupSlider(&speedSlider, &otherLookAndFeel2, speedSliderParams);
        setupLabel(&speedSlider, &speedLabel, "Speed");

        SliderParams dampingSliderParams = {
            .range = {1, 1000.0},
            .defaultValue = 10.0,
            .numDecimalPlaces = 2,
            .style = juce::Slider::LinearHorizontal,
            .textValueSuffix = new std::string(" factor"),
        };

        setupSlider(&dampingSlider, &otherLookAndFeel3, dampingSliderParams);
        setupLabel(&dampingSlider, &dampingLabel, "Damping");

        SliderParams positionSliderParams = {
            .range = {0.0, 100.0},
            .defaultValue = 10.0,
            .numDecimalPlaces = 2,
            .style = juce::Slider::LinearHorizontal,
            .textValueSuffix = new std::string(" % of track"),
        };

        setupSlider(&positionSlider, &otherLookAndFeel3, positionSliderParams);
        setupLabel(&positionSlider, &positionLabel, "Position");
        
        // Setup label for knobs

        SliderParams freqKnobParam = {
            .range = {0.0, 100.0},
            .defaultValue = 50.0,
            .numDecimalPlaces = 2,
            .style = juce::Slider::Rotary,
            .textValueSuffix = new std::string(" % of track"),
        };

        addAndMakeVisible(lowSlider);
        setupSlider(&lowSlider, &otherLookAndFeel3, freqKnobParam);

        addAndMakeVisible(midSlider);
        setupSlider(&midSlider, &otherLookAndFeel3, freqKnobParam);

        addAndMakeVisible(highSlider);
        setupSlider(&highSlider, &otherLookAndFeel3, freqKnobParam);

        addAndMakeVisible(waveDisplay);

        startTimer(500);
}

AssemblePane::~AssemblePane() {};

void AssemblePane::setupButton(juce::Button* component) {
        addAndMakeVisible(component);
        component->addListener(this);
};

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

        if (params.textValueSuffix != nullptr) {
                component->setTextValueSuffix(*params.textValueSuffix);
                DBG(*params.textValueSuffix);
        }
};

void AssemblePane::setupLabel(juce::Component* target, juce::Label* label, std::string text) {
        addAndMakeVisible(label);
        label->setText(text, juce::dontSendNotification);
        label->attachToComponent(target, true);
};

void AssemblePane::paint(juce::Graphics& g) {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */
        g.fillAll(juce::Colour{62, 95, 138});

        g.setColour(juce::Colours::grey);
        g.drawRect(getLocalBounds(), 1);

        g.setColour(juce::Colours::navajowhite);
        g.setFont(30.0f);
        g.drawText("DJ Audio", 10, 10, getWidth(), 10, juce::Justification::centred, true);
}

void AssemblePane::resized() {
        int rowHeight = getHeight() / 15;
        double width = (getWidth() - 20);
        int sliderLeftMargin = 60;

        auto border = 4;

        auto area = getLocalBounds();

        auto dialArea = area.removeFromTop(area.getHeight() * 2);

        double thirdWidth = width / 3;

        playButton.setBounds(15, rowHeight, thirdWidth - 10, rowHeight / 2);
        stopButton.setBounds(thirdWidth + 15, rowHeight, thirdWidth - 10, rowHeight / 2);
        loadButton.setBounds(2 * thirdWidth + 15, rowHeight, thirdWidth - 10, rowHeight / 2);

        volSlider.setBounds(sliderLeftMargin, 10 + playButton.getBounds().getBottom(), width / 2 - sliderLeftMargin,
                            rowHeight * 2);
        speedSlider.setBounds(sliderLeftMargin + width / 2, 10 + playButton.getBounds().getBottom(),
                              width / 2 - sliderLeftMargin, rowHeight * 2);
        positionSlider.setBounds(sliderLeftMargin, 10 + speedSlider.getBounds().getBottom(), width - sliderLeftMargin,
                                 rowHeight);
        dampingSlider.setBounds(sliderLeftMargin, 10 + positionSlider.getBounds().getBottom(), width - sliderLeftMargin,
                                rowHeight);

        // TODO: draw mid/low/high sliders
        // render in a row view all of them V
        lowSlider.setBounds(sliderLeftMargin, 10 + positionSlider.getBounds().getBottom(), width / 3 - sliderLeftMargin, rowHeight * 2);
        midSlider.setBounds(lowSlider.getBounds().getRight(), 10 + positionSlider.getBounds().getBottom(), width / 3 - sliderLeftMargin, rowHeight * 2);
        highSlider.setBounds(midSlider.getBounds().getRight(), 10 + positionSlider.getBounds().getBottom(), width / 3 - sliderLeftMargin, rowHeight * 2);

        waveDisplay.setBounds(5, 8 * (getHeight() / 10), width + 10, rowHeight * 2.2);
}

// intended to handle button click listener events
void AssemblePane::buttonClicked(juce::Button* button) {
        if (button == &playButton) {
                std::cout << "Play button clicked" << std::endl;
                int secs = player->getLengthInSeconds();
                DBG("Length of track: " << secs);
                // player calls start function from DJaudio
                player->start();
        }
        if (button == &stopButton) {
                player->stop();
        }
        if (button == &loadButton) {
                // juce::FileChooser fChooser{"Select a file..."};
                constexpr auto folderChooserFlags = FileBrowserComponent::canSelectFiles |
                                                    FileBrowserComponent::openMode;

                fChooser.launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser) {
                        const juce::File file(chooser.getResult());
                        DBG(file.getFullPathName());
                        loadFile(juce::URL{file});
                });
        }
}

void AssemblePane::sliderValueChanged(juce::Slider* slider) {
        double value = slider->getValue();
        if (slider == &volSlider) {
                std::cout << value << std::endl;
                std::cout << "Vol Slider handle" << std::endl;
                player->setGain(value);
                std::cout << "After Vol Slider handle" << std::endl;
        }

        if (slider == &speedSlider) {
                std::cout << "Speed Slider changed" << value << std::endl;
                player->setSpeed(value);
        }

        if (slider == &positionSlider) {
                std::cout << "Position Slider changed" << value << std::endl;
                if ((value - player->getPositionRelative()) > 2) {
                        player->setPositionRelative(value / 100);
                } else if (player->getPositionRelative() - value > 2) {
                        player->setPositionRelative(value / 100);
                }
        }

        if (slider == &dampingSlider) {
                DBG("Damping Slider changed" << slider->getValue());
        }
}

bool AssemblePane::isInterestedInFileDrag(const juce::StringArray& files) { return false; }

void AssemblePane::filesDropped(const juce::StringArray& files, int, int y) {
        DBG("Files dropped");
        if (files.size() == 1) {
                loadFile(juce::URL{files[0]});
        }
}

void AssemblePane::timerCallback() {
        positionSlider.setValue(player->getPositionRelative());
        waveDisplay.setPositionRelative(player->getPositionRelative() * 10);
}

void AssemblePane::loadFile(juce::URL audioURL) {
        DBG("AssemblePane::loadFile called");
        DBG(audioURL.toString(true));
        player->loadUrl(audioURL);
        waveDisplay.loadURL(audioURL);
}
