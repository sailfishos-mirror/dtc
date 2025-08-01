project('dtc', 'c',
  version: files('VERSION.txt'),
  license: ['GPL2+', 'BSD-2'],
  default_options: ['werror=true', 'default_library=both'],
  meson_version: '>=0.57.0'
)

cc = meson.get_compiler('c')

add_project_arguments(
  cc.get_supported_arguments([
    '-Wpointer-arith',
    '-Wcast-qual',
    '-Wnested-externs',
    '-Wstrict-prototypes',
    '-Wmissing-prototypes',
    '-Wredundant-decls',
    '-Wshadow',
    '-Wsuggest-attribute=format',
    '-Wwrite-strings',
  ]),
  language: 'c'
)

add_project_arguments(
  '-DFDT_ASSUME_MASK=' + get_option('assume-mask').to_string(),
  language: 'c'
)

yamltree = 'yamltree.c'
yaml = dependency('yaml-0.1', version: '>=0.2.3', required: get_option('yaml'))
if not yaml.found()
  add_project_arguments('-DNO_YAML', language: 'c')
  yamltree = []
endif

valgrind = dependency('valgrind', required: get_option('valgrind'))
if not valgrind.found()
  add_project_arguments('-DNO_VALGRIND', language: 'c')
endif

py = import('python')
py = py.find_installation(required: get_option('python'))
swig = find_program('swig', required: get_option('python'))
pylibfdt_enabled = not meson.is_cross_build() and py.found() and swig.found() ? true : false
wheel_only = get_option('wheel-only')

version_gen_h = vcs_tag(
  command: ['git', 'describe', '--dirty=+'],
  input: 'version_gen.h.in',
  output: 'version_gen.h',
)

subdir('libfdt')

dtc_tools = []
util_dep = declare_dependency(
  sources: ['util.c', version_gen_h],
  include_directories: '.',
  dependencies: libfdt_dep
)

if get_option('tools') and not wheel_only
  flex = find_program('flex', required: true)
  bison = find_program('bison', required: true)

  lgen = generator(
    flex,
    output: '@PLAINNAME@.lex.c',
    arguments: ['-o', '@OUTPUT@', '@INPUT@'],
  )

  pgen = generator(
    bison,
    output: ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
    arguments: ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@'],
  )

  if cc.check_header('fnmatch.h')
    dtc_tools += executable(
      'convert-dtsv0',
      [
        lgen.process('convert-dtsv0-lexer.l'),
        'srcpos.c',
      ],
      dependencies: util_dep,
      install: true,
    )
  endif

  dtc_tools += executable(
    'dtc',
    [
      lgen.process('dtc-lexer.l'),
      pgen.process('dtc-parser.y'),
      'checks.c',
      'data.c',
      'dtc.c',
      'flattree.c',
      'fstree.c',
      'livetree.c',
      'srcpos.c',
      'treesource.c',
      yamltree,
    ],
    dependencies: [util_dep, yaml],
    install: true,
  )

  foreach e: ['fdtdump', 'fdtget', 'fdtput', 'fdtoverlay']
    dtc_tools += executable(e, files(e + '.c'), dependencies: util_dep, install: true)
  endforeach

  install_data(
    'dtdiff',
    install_dir: get_option('bindir'),
    install_mode: 'rwxr-xr-x',
  )
endif

foreach e: dtc_tools
  meson.override_find_program(e.name(), e)
endforeach

if pylibfdt_enabled
  subdir('pylibfdt')
endif

if get_option('tests')
  subdir('tests')
endif
