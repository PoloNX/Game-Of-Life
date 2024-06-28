add_rules("mode.debug", "mode.release")
add_rules("plugin.vsxmake.autoupdate")


add_requires("libsdl 2")
add_requires("imgui", { configs = { sdl2 = true}})
add_ldflags("/subsystem:console")

target("Game Of Life")
    set_kind("binary")
    set_languages("cxx20")

    add_files("source/*.cpp")
    add_headerfiles("include/*.hpp")
    add_includedirs("include")

    set_rundir(".")

    add_packages("libsdl", "imgui")
