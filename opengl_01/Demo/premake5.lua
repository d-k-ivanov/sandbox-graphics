project "Demo"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    files
    {
        "**.cpp",
        "**.h",
        "**.lua",
        "**.md",
    }

    vpaths
    {
        ["Archive/*"]   = {"**.md"},
        ["Build/*"]     = {"**.lua"},
        ["Headers/*"]   = { "**.h", "**.hpp" },
        ["Sources/*"]   = {"**.c", "**.cpp"},
    }

    filter ({})
