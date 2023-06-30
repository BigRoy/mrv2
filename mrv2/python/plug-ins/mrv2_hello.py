# SPDX-License-Identifier: BSD-3-Clause
# mrv2
# Copyright Contributors to the mrv2 Project. All rights reserved.

#
# For this example, we will just use the timeline module.
#
import mrv2
from mrv2 import timeline

#
# Main Plugin class.  Must exist for the plugin to be created.
#
class Plugin:
    """
    Constructor.  Init your variables here.
    """
    def __init__(self):
        pass

    """
    Status of the plug-in.  Must be True of False.
    Change this to False to not activate the plug-in.
    If active is missing from the class, it is assumed to be True.
    """
    def active(self):
        return True

    """
    The actual plugin execution for Python/Playback/Forwards.
    """
    def play(self):
        timeline.playForwards()
    
    """
    The actual plugin execution for Python/Hello.
    """
    def run(self):
        print("Hello from python plugin!")

        
    """
    Return the menus to add to mrv2's main toolbar.
    You can add your commands to mrv2 menus or create a new
    menu entry. Each Submenu is separated by '/'.
    Currently, you cannot remove a menu entry.

    Returns:
    dict: a dictionary of tuples.

    The first entry of the tuple is the menu entry to add and the
    second entry is the method or function to run.
    """
    def menus(self):
        menus = {
            ("Python/Hello", self.run),
            ("Python/Play/Forwards", self.play),              # call a method
            ("Python/Play/Backwards", timeline.playBackwards) # call a function
        }
        return menus
        