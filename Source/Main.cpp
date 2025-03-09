/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include <memory>
#include "MainComponent.h"

//==============================================================================
class OtoDeckApplication : public juce::JUCEApplication {
       public:
        //==============================================================================
        OtoDeckApplication() {}

        // Returns the application name as defined in the project info
        const juce::String getApplicationName() override { return ProjectInfo::projectName; }

        // Returns the application version as defined in the project info
        const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }

        // Allows multiple instances of the application to run at once
        bool moreThanOneInstanceAllowed() override { return true; }

        //==============================================================================
        void initialise(const juce::String& commandLine) override {
                // This is where your application's initialization code goes.
                // Here we initialize the main window.
                mainWindow.reset(new MainWindow(getApplicationName()));
        }

        void shutdown() override {
                // Shutdown code to clean up when the application closes
                mainWindow = nullptr;        // (deletes our window)
        }

        //==============================================================================
        void systemRequestedQuit() override {
                // This method is called when the system requests the app to quit.
                // Here, we simply call quit() to close the app.
                quit();
        }

        void anotherInstanceStarted(const juce::String& commandLine) override {
                // If another instance of the app is started while this one is running,
                // this method is invoked with the command-line arguments of the new instance.
        }

        //==============================================================================
        /*
            This class implements the desktop window that contains an instance of
            our MainComponent class.
        */
        class MainWindow : public juce::DocumentWindow {
               public:
                MainWindow(juce::String name)
                    : DocumentWindow(name,
                                     juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(
                                         juce::ResizableWindow::backgroundColourId),
                                     DocumentWindow::allButtons) {
                        // Setting up the title bar and content for the window
                        setUsingNativeTitleBar(true);
                        setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
                        // If the platform is iOS or Android, set the window to fullscreen
                        setFullScreen(true);
#else
                        // For other platforms, make the window resizable and center it
                        setResizable(true, true);
                        centreWithSize(getWidth(), getHeight());
#endif

                        // Make the window visible after setup
                        setVisible(true);
                }

                void closeButtonPressed() override {
                        // This method is called when the user tries to close the window.
                        // Here, it requests the app to quit.
                        JUCEApplication::getInstance()->systemRequestedQuit();
                }

                /* 
                   Note: Be cautious when overriding any DocumentWindow methods as 
                   the base class uses many of them. Ensure that any overridden methods
                   still call the superclass's method to maintain functionality.
                */

               private:
                // Prevent copying and assigning of the MainWindow object
                JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
        };

       private:
        // A unique pointer to manage the MainWindow object
        std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(OtoDeckApplication)
