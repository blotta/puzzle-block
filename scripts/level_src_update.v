import os

fn main() {
	mut levels := []string{}

	mut lvl_files := os.glob('./assets-build/levels/*.txt')!
	lvl_files.sort_with_compare(lvl_file_sort)

	for lvl in lvl_files {
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

fn lvl_file_sort(a &string, b &string) int {
	na := os.file_name(a).all_before_last('.').int()
	nb := os.file_name(b).all_before_last('.').int()
	return na - nb
}