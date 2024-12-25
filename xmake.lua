set_languages("c++20")

set_warnings("all", "error")

add_requires("libsdl", "libsdl_image", "libsdl_ttf")

add_rules("mode.debug", "mode.release")

if is_mode("debug") then
    add_defines("DEBUG")
elseif is_mode("release") then
    add_defines("NDEBUG")
    set_optimize("fast")
end

target("game")
    set_kind("binary")
    add_packages("libsdl", "libsdl_image", "libsdl_ttf")
    add_files("src/*.cpp")
    if is_mode("debug") then
        -- add_ldflags("cl::/SUBSYSTEM:CONSOLE")
        if is_subhost("windows") then
            add_ldflags("/SUBSYSTEM:CONSOLE")
            -- add_cxxflags("cl::/SUBSYSTEM:CONSOLE", {force = true}) -- only on msvc
        end
    end