
levels = Dir.glob("./assets/levels/*.txt")

puts levels

File.open("./assets/levels/gen_level_data.hpp", "w") {|f|
    f.write("#define NUM_LEVELS #{levels.length}\n")
}

File.open("./assets/levels/gen_level_data.cpp.part", "w") {|f|
    levels.each_with_index do |level,idx|
        f.write(
            "\ndata->DefaultLevels[#{idx}] = \n"
        )
        f.write(
            File.read(level)
        )
        f.write(";\n")
    end
}
