# Setup Vscode debug

## Ref
Debugger: [Github](https://github.com/moteus/vscode-mobdebug/tree/main/sampleWorkspace)  
Followed: [Link](https://robertgpatterson.com/-fininfo/-rgplua/docs/rgp-lua/development-environment.html)

Plugins
1. [Server](https://marketplace.visualstudio.com/items?itemName=sumneko.lua)
2. [Debugger](https://marketplace.visualstudio.com/items?itemName=AlexeyMelnichuk.lua-mobdebug)

## file changes
Then open the Koreader repo from VScode

```lua
-- Debugger
local home = os.getenv("HOME") or os.getenv("HOMEDRIVE") .. os.getenv("HOMEPATH")
package.path = home .. "/.vscode-oss/extensions/alexeymelnichuk.lua-mobdebug-0.0.5/lua/?.lua"
    .. ";" .. package.path
local mobdebug = require("vscode-mobdebug")
mobdebug.start('127.0.0.1', 8172)

local print = function(...) print(...); io.flush() end -- Immediate print output
print("Debugger connected successfully!")
mobdebug.off()
-- Debugger
```

Add the Above code snippet in staring of the required debugging file

In above `package.path = home .. "/.vscode-oss/extensions/alexeymelnichuk.lua-mobdebug-0.0.5/lua/?.lua"`  
path depends up on the os and place where extension found, for linux mostly `package.path = home .. "/.vscode-oss/extensions/alexeymelnichuk.lua-mobdebug-0.0.5/lua/?.lua"` or `package.path = home .. "/.vscode/extensions/alexeymelnichuk.lua-mobdebug-0.0.5/lua/?.lua"`    
Main idea is to loacte the mobdebug from vscode extension

`mobdebug.off()` is added because it take too much time to load while debugger is enabled

where we want to debug there we need to enable the debug
```lua
mobdebug.on()
print("self.ui:", self.ui)
for k, v in pairs(self.ui) do
    print(k, v)
end
-- local doc = UIManager:getDocument()
-- print("UI manager : ", doc)     
page_num = Reader:getCurrentPage()

print("Hi ", page_num)
mobdebug.off()
```

The above 2 code snippet belongs to the same file

Example:
```lua
-- Debugger
local home = os.getenv("HOME") or os.getenv("HOMEDRIVE") .. os.getenv("HOMEPATH")
package.path = home .. "/.vscode-oss/extensions/alexeymelnichuk.lua-mobdebug-0.0.5/lua/?.lua"
    .. ";" .. package.path
local mobdebug = require("vscode-mobdebug")
mobdebug.start('127.0.0.1', 8172)

local print = function(...) print(...); io.flush() end -- Immediate print output
print("Debugger connected successfully!")
mobdebug.coro()
mobdebug.off()
-- Debugger


local Dispatcher = require("dispatcher")  -- luacheck:ignore
local InfoMessage = require("ui/widget/infomessage")
local UIManager = require("ui/uimanager")
local WidgetContainer = require("ui/widget/container/widgetcontainer")
local Reader = require("apps/reader/readerui")

local _ = require("gettext")

local ReadAloud = WidgetContainer:extend{
    name = "readaloud",
    is_doc_only = false,
}

function ReadAloud:onDispatcherRegisterActions()
    Dispatcher:registerAction("readaloud_action", {category="none", event="ReadAloud", title=_("Read Aloud"), general=true,})
end

function ReadAloud:init()
    self:onDispatcherRegisterActions()
    self.ui.menu:registerToMainMenu(self)
end

function ReadAloud:addToMainMenu(menu_items)
    menu_items.ReadAloud_world = {
        text = _("Read Aloud"), -- title to appear in screen
        -- in which menu this should be appended
        sorting_hint = "setting",--"more_tools",

        -- a callback when tapping
        callback = function()
           
            mobdebug.on()
            UIManager:show(InfoMessage:new{
                text = _("ReadAloud, Test "),
            })
            mobdebug.off()
        end,
    }
end

function ReadAloud:onReadAloudWorld()
    local popup = InfoMessage:new{
        text = _("ReadAloud World"),
    }
    UIManager:show(popup)
end

return ReadAloud

```

## Debug config
create vscode debug config in `.vscode/launch.json`

```json
{
    "version": "0.2.0",
    "configurations": [

        {
            "name": "Lua MobDebug: Listen",
            "type": "luaMobDebug",
            "request": "attach",
            "workingDirectory": "${workspaceFolder}",
            "sourceBasePath": "${workspaceFolder}",
            "listenPublicly": false,
            "listenPort": 8172,
            "stopOnEntry": true,
            "sourceEncoding": "UTF-8"

        },
    ]
}
```

## How to run

1. Start the degugger by clicking debug icon which name is `Lua MobDebug: Listen`
2. run `./kodev run`