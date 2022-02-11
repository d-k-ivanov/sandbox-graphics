require('vstudio')

premake.api.register {
    name = "solution_items",
    scope = "workspace",
    kind = "list:string",
}

premake.override(premake.vstudio.sln2005, "projects", function(base, wks)
    if wks.solution_items and #wks.solution_items > 0 then
        local solution_folder_GUID = "{2150E333-8FDC-42A3-9474-1A3956D46DE8}" -- See https://www.codeproject.com/Reference/720512/List-of-Visual-Studio-Project-Type-GUIDs
        premake.push("Project(\"" .. solution_folder_GUID .. "\") = \"Solution Items\", \"Solution Items\", \"{" .. os.uuid("Solution Items:" .. wks.name) .. "}\"")
        premake.push("ProjectSection(SolutionItems) = preProject")

        for _, path in ipairs(wks.solution_items) do
            premake.w(path .. " = " .. path)
        end

        premake.pop("EndProjectSection")
        premake.pop("EndProject")
    end
    base(wks)
end)

local function include_conan(filename)
    include(filename)
    local buildinfo = { }
    local prefix = 'conan_'
    for k, v in pairs(_G) do
        if k:sub(1, #prefix) == prefix then
            buildinfo[k:sub(#prefix + 1)] = v
        end
    end
    return buildinfo
end

local function conan_setup(include_conan)
    includedirs { include_conan.includedirs }
    libdirs     { include_conan.libdirs }
    links       { include_conan.libs }
    links       { include_conan.system_libs }
    links       { include_conan.frameworks }
    defines     { include_conan.defines }
    bindirs     { include_conan.bindirs }
end

conan_debug          = include_conan "conan_debug/conanbuildinfo.premake.lua"
conan_release        = include_conan "conan_release/conanbuildinfo.premake.lua"
conan_relwithdebinfo = include_conan "conan_relwithdebinfo/conanbuildinfo.premake.lua"

-- include("conanbuildinfo.premake.lua")

workspace "OpenGL"
    -- conan_basic_setup()
    startproject "BezierCurve"

    configurations { "RelWithDebInfo", "Release", "Debug" }
    platforms { "x64", "x86"}
    warnings "Extra"

    flags {"MultiProcessorCompile", "ShadowedVariables", "UndefinedIdentifiers"}

    targetdir ("bin/%{prj.name}/%{cfg.buildcfg}-%{cfg.architecture}")
    debugdir ("bin/%{prj.name}/%{cfg.buildcfg}-%{cfg.architecture}")
    objdir ("bin-int/%{prj.name}/%{cfg.buildcfg}-%{cfg.architecture}")

    -- targetdir "%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}"
    -- debugdir "%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}"
    -- objdir "!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}"

    filter "platforms:x86"
        architecture "x86"

    filter "platforms:x64"
        architecture "x86_64"

    filter({"platforms:x86","system:windows"})
        defines({"COMPILER_MSVC32","WIN32"})

    filter({"platforms:x86_64","system:windows"})
        defines({"COMPILER_MSVC64","WIN64"})

    filter "configurations:RelWithDebInfo"
        defines {"NDEBUG", "RELEASE", "_RELEASE"}
        optimize "Debug"
        runtime "Release"
        symbols "On"
        conan_setup(conan_relwithdebinfo)
        -- libdirs { conan_relwithdebinfo.libdirs }

    filter "configurations:Release"
        defines {"NDEBUG", "RELEASE", "_RELEASE"}
        flags "LinkTimeOptimization"
        optimize "Full"
        runtime "Release"
        symbols "Off"
        conan_setup(conan_release)
        -- libdirs { conan_release.libdirs }

    filter "configurations:Debug"
        defines {"DEBUG", "_DEBUG"}
        optimize "Off"
        runtime "Debug"
        symbols "On"
        conan_setup(conan_debug)
        -- libdirs { conan_debug.libdirs }

    filter "system:windows"
        cdialect "C17"
        cppdialect "C++20"
        debuggertype "NativeOnly"
        defaultplatform "x64"
        defines {"_CRT_NONSTDC_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "STRICT", "COMPILER_MSVC" }
        staticruntime "On"


    filter "system:linux"
        cdialect "gnu17"
        cppdialect "gnu++20"
        staticruntime "Off"
        defaultplatform "x64"
        linkoptions "-Wl,--no-undefined"
        defines({ "LINUX", "_LINUX", "COMPILER_GCC", "POSIX" })

    filter "files:**.c or **.cc or **.cpp or **.cxx"
        strictaliasing "Level3"

    filter({})

include "BezierCurve"
include "BezierSurface"
include "NURBSSurface"

project "Other"
    kind "None"

    files
    {
        ".gitignore",
        ".editorconfig",
        "conanfile.txt",
        "commit_now.ps1",
        "conan_init.ps1",
        "conan_debug/conanbuildinfo.premake.lua",
        "conan_release/conanbuildinfo.premake.lua",
        "conan_relwithdebinfo/conanbuildinfo.premake.lua",
        "gen_solution.bat",
        "premake5.lua"
    }

    vpaths
    {
        ["Build/*"] = {
            "**.lua",
            "**.bat",
            "conan_init.ps1",
            "conanfile.txt",
            "conanbuildinfo.premake.lua",
            "conan_debug/conanbuildinfo.premake.lua",
            "conan_release/conanbuildinfo.premake.lua",
            "conan_relwithdebinfo/conanbuildinfo.premake.lua"
        },
        ["Git/*"]   = {
            ".gitignore",
            "commit_now.ps1",
            ".editorconfig"
        }
    }

    filter ({})
