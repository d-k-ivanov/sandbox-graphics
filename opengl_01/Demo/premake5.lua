project "Demo"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
    entrypoint "mainCRTStartup"

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
