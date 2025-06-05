/**
 * @file ConfigMode.h
 * @brief Application mode for accessing and modifying system configuration.
 *
 * This mode presents the ConfigScreen and allows users to adjust persistent
 * settings such as control mappings and global behavior.
 */

#pragma once

#include "modes/AppMode.h"
#include "screens/ConfigScreen.h"


/**
 * @class ConfigMode
 * @brief Mode that enables editing of system configuration parameters.
 *
 * Provides access to the ConfigScreen where users can view and modify control
 * mappings and other persistent system settings.
 */
class ConfigMode : public AppMode {
private:
    ConfigScreen theConfigScreen;   ///< The screen used to modify system settings

public:

    /**
     * @brief Returns the name of the mode.
     * @return String name for display
     */
    const char* name() const override { return "System Config Mode"; }

    /**
     * @brief Returns the screen associated with this mode.
     * @return Pointer to the ConfigScreen instance
     */
    Screen* getScreen() override { return &theConfigScreen; }
    
    /**
     * @brief Called when the mode is activated.
     *
     * Initializes the configuration screen and prepares it for user interaction.
     */
    void onEnter() override;

    /**
     * @brief Called when the mode is exited.
     *
     * Cleans up any resources or state related to the configuration screen.
     */
    void onExit() override;

    /**
     * @brief Executes logic that should run continuously during the mode.
     *
     * This method is called in the main loop to handle input and update the screen.
     */
    void loop() override;

    /**
     * @brief Displays the configuration screen.
     */
    void showScreen() override;
};
