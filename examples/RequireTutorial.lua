--[[
    This tutorial explains how to use Extra Utilities
    in your mods

    By VTrider
--]]

-- This module is necessary in order to fix the lua module loader
-- to search for dll files, load it first.
local RequireFix = require("RequireFix")

-- Initialize RequireFix with the workshop ID of your mod.
-- This uses the Extra Utilities workshop item as an example.
-- Upload the mod first to get a workshop ID, then update your script.
local yourWorkshopID = "3144751838"

RequireFix.Initialize(yourWorkshopID)

-- Alternatively if you are testing locally, you can set your
-- package.cpath to the absolute path to the dll.
package.cpath = "C:\\Your\\path\\to\\the\\dll\\?.dll"
-- Note the double backslashes which are necessary on windows.
-- As well as the final entry '?.dll', this tells it to search for
-- dll files specifically in that folder.

-- You're now ready to load and uses the main module.
local exu = require("exu")

-- For more information and function documentation, see the wiki: https://github.com/VTrider/ExtraUtilities/wiki