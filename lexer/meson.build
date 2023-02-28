project('lexer', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

incdir = include_directories('include')

executable('lexer',
           'main.cpp',
           'preprocessor.cpp',
           'showtokens.cpp',
           'lexer_dfa.cpp',
           'Token.cpp',
           'parser.cpp',
           install : false,
           include_directories : incdir)
