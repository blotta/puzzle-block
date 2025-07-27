import os

fn main() {
	mut levels := []string{}

	for lvl in os.glob('./assets-build/levels/*.txt')! {
		println(lvl)
		levels << os.read_file('./assets-build/levels/${lvl}')!
	}

	println('./assets-build/levels/gen_level_data.hpp')
	os.write_lines('./assets-build/levels/gen_level_data.hpp', [
		'#define NUM_LEVELS ${levels.len}',
	])!

	println('./assets-build/levels/gen_level_data.cpp.part')
	mut content := ''
	for idx, lvl in levels {
		content += '\ndata->DefaultLevels[${idx}] =\n${lvl};\n'
	}
	os.write_lines('./assets-build/levels/gen_level_data.cpp.part', [content])!

	println('written ${levels.len} levels')
}
