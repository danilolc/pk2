#!/usr/bin/env lua5.4

--[[

Pekka Kana 2
The original game by Janne Kivilahti (2003-2007)
and the Piste Gamez community.

Currently maintained by SaturninTheAlien.
See the GitHub page for more information:
https://github.com/SaturninTheAlien/pk2_greta

]]


local lib_dir = "/usr/lib/games/pekka-kana-2/"
local assets_dir = "/usr/share/games/pekka-kana-2/"
local pk2_realBinName = "pk2_greta_1.5.1"


local args = { ... }
local has_assets_path = false
local has_data_path = false
local has_help = false

for _, arg in ipairs(args) do
    if arg=="--assets-path" then
        has_assets_path = true

    elseif arg=="--data-path" then
        has_data_path = true

    elseif arg=="--help" or arg=="-h" or arg=="--version" or arg=="-v" then
        has_help = true
    end
end


if not has_help then

    if not has_assets_path then
        table.insert(args, "--assets-path")
        table.insert(args, assets_dir)
    end

    if not has_data_path then
        table.insert(args, "--data-path")
        table.insert(args, "PREF_PATH")
    end
end


local ld_library_path = os.getenv("LD_LIBRARY_PATH")
if ld_library_path == nil or ld_library_path == "" then
    ld_library_path = lib_dir
else
    ld_library_path = ld_library_path .. ":" .. lib_dir
end

local command = "export LD_LIBRARY_PATH=\""..ld_library_path.."\" && "..
    lib_dir.. pk2_realBinName.. " " .. table.concat(args, " ")



-- print(command)
local ret = os.execute(command)
os.exit(ret)

--[[
   Lua Cultist was here!
   Glory to the Moon! 🌜
]]
