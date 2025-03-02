
#include "AssesmblePane.h"

#include <memory>

#include "juce_core/system/juce_PlatformDefs.h"
#include "juce_gui_basics/juce_gui_basics.h"

AssemblePane::AssemblePane(AudioPlayer* _player, juce::AudioFormatManager& formatManagerToUse,
                           juce::AudioThumbnailCache& cacheToUse)
    : player(_player) {
        // In your constructor, you should add any child components, and initialise any special settings that your
        // component needs.

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

        addAndMakeVisible(volLabel);
        volLabel.setText("Volume", juce::dontSendNotification);
        volLabel.attachToComponent(&volSlider, true);

        SliderParams speedSliderParams = {
            .range = {0.1, 2.0},
            .defaultValue = 1.0,
            .numDecimalPlaces = 2,
            .style = juce::Slider::Rotary,
        };

        setupSlider(&speedSlider, &otherLookAndFeel2, speedSliderParams);

        // speedSlider.setLookAndFeel(&otherLookAndFeel2);
        // speedSlider.setSliderStyle(juce::Slider::Rotary);
        // speedSlider.addListener(this);        // thing that wants to receive the events needs to tell the GUI object
        //                                       // that is wants to register for events
        // speedSlider.setRange(0.1, 2.0);
        // speedSlider.setValue(1.0);
        // speedSlider.setNumDecimalPlacesToDisplay(2);
        // speedSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 60, speedSlider.getTextBoxHeight());

        addAndMakeVisible(speedLabel);
        speedLabel.setText("Speed", juce::dontSendNotification);
        speedLabel.attachToComponent(&speedSlider, true);

        SliderParams dampingSliderParams = {
            .range = {1, 1000.0},
            .defaultValue = 10.0,
            .numDecimalPlaces = 2,
            .style = juce::Slider::LinearHorizontal,
            .textValueSuffix = new std::string(" factor"),
        };

        setupSlider(&dampingSlider, &otherLookAndFeel3, dampingSliderParams);

        // addAndMakeVisible(dampingSlider);
        // dampingSlider.setRange(1, 1000.0);
        // dampingSlider.setTextValueSuffix(" factor");
        // dampingSlider.setValue(10.0);
        // dampingSlider.addListener(this);
        // dampingSlider.setSkewFactorFromMidPoint(500);
        // dampingSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 120, dampingSlider.getTextBoxHeight());
        // addAndMakeVisible(dampingLabel);
        // dampingLabel.setText("Damping", juce::dontSendNotification);
        // dampingLabel.attachToComponent(&dampingSlider, true);

        addAndMakeVisible(dampingLabel);
        dampingLabel.setText("Damping", juce::dontSendNotification);
        dampingLabel.attachToComponent(&dampingSlider, true);

        SliderParams positionSliderParams = {
            .range = {0.0, 1.0},
            .defaultValue = 10.0,
            .numDecimalPlaces = 2,
            .style = juce::Slider::LinearHorizontal,
            .textValueSuffix = new std::string(" % of track"),
        };

        setupSlider(&positionSlider, &otherLookAndFeel3, positionSliderParams);

        addAndMakeVisible(positionLabel);
        positionLabel.setText("Position", juce::dontSendNotification);
        positionLabel.attachToComponent(&positionSlider, true);

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

void AssemblePane::paint(juce::Graphics& g) {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the
        // background
        g.fillAll(juce::Colour{62, 95, 138});

        g.setColour(juce::Colours::grey);
        g.drawRect(getLocalBounds(), 1);        // draw an outline around the component

        g.setColour(juce::Colours::navajowhite);
        g.setFont(30.0f);
        // g.drawText ("DJ Audio", getLocalBounds(),juce::Justification::centred, true);   // draw some placeholder text
        g.drawText("DJ Audio", 10, 10, getWidth(), 10, juce::Justification::centred,
                   true);        // draw some placeholder text
}

void AssemblePane::resized() {
        // This method is where you should set the bounds of any child components that your component contains..

        double rowHeight = getHeight() / 12;
        double width = (getWidth() - 20);
        auto sliderLeft = 60;

        auto border = 4;

        auto area = getLocalBounds();

        auto dialArea = area.removeFromTop(area.getHeight() * 2);

        playButton.setBounds(15, 40, (width / 3) - 10, rowHeight);
        stopButton.setBounds((width / 3) + 15, 40, (width / 3) - 10, rowHeight);
        loadButton.setBounds(2 * (width / 3) + 15, 40, (width / 3) - 10, rowHeight);

        // setColour(juce::Slider::thumbColourId, juce::Colours::red);
        volSlider.setBounds(sliderLeft, 10 + (rowHeight * 2), width / 2 - sliderLeft, rowHeight * 4);
        speedSlider.setBounds(sliderLeft + width / 2, 10 + (rowHeight * 2), width / 2 - sliderLeft, rowHeight * 4);
        positionSlider.setBounds(sliderLeft, 10 + (rowHeight * 5), width - sliderLeft, rowHeight * 2);
        dampingSlider.setBounds(sliderLeft, 10 + (rowHeight * 7), width - sliderLeft, rowHeight * 2);
}

// intended to handle button click listener events
void AssemblePane::buttonClicked(juce::Button* button) {
        if (button == &playButton) {
                std::cout << "Play button clicked" << std::endl;

                // player calls setPosition(0) function from DJaudio to start playing from the beginning
                player->setPosition(0);

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
        if (slider == &volSlider) {
                std::cout << slider->getValue() << std::endl;
                std::cout << "Vol Slider handle" << std::endl;
                player->setGain(slider->getValue());
                std::cout << "After Vol Slider handle" << std::endl;
        }
        if (slider == &speedSlider) {
                std::cout << "Speed Slider changed" << slider->getValue() << std::endl;
                player->setSpeed(slider->getValue());
        }
        if (slider == &positionSlider) {
                std::cout << "Position Slider changed" << slider->getValue() << std::endl;
                player->setPosition(slider->getValue());
        }

        if (slider == &dampingSlider) {
                DBG("Damping Slider changed" << slider->getValue());
        }
}

bool AssemblePane::isInterestedInFileDrag(const juce::StringArray& files) { return false; }

void AssemblePane::filesDropped(const juce::StringArray& files, int, int y) {
        DBG("Files dropped");
        if (files.size() == 1) {
                player->loadUrl(juce::URL{juce::File{files[0]}});        // player calls loadURL function from DJaudio
                // waveFormDisplay.loadURL(juce::URL{ fChooser.getResult() });
        }
}

void AssemblePane::timerCallback() {}

void AssemblePane::loadFile(juce::URL audioURL) {
        DBG("DeckGUI::loadFile called");
        player->loadUrl(audioURL);
}
