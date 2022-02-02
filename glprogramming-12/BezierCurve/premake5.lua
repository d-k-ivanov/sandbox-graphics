project "BezierCurve"
    kind "ConsoleApp"
    language "C"
    cdialect  "C17"

    files
    {
        "**.c",
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
