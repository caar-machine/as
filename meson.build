project('caar-as', 'c')

inc = include_directories('src')

input_files = [
'src/as.h',
'src/as.c',
'src/ast.h',
'src/ast.c',
'src/codegen.h',
'src/codegen.c',
'src/lex.h',
'src/lex.c',
'src/main.c',
'src/lib/log.h',
'src/lib/log.c',
'src/lib/map.h',
'src/lib/map.c',
'src/lib/vec.h',
'src/lib/vec.c',
]

install_dir = join_paths(meson.current_source_dir(), 'bin')

if meson.is_subproject()
  install_dir = join_paths(meson.current_source_dir(), '../../bin')
endif


assembler_executable = executable('caar-as', sources: input_files, install: true, install_dir: install_dir, include_directories: inc)
