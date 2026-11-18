project('SecretGiver', 'cpp', version: '1.0.0', default_options: ['cpp_std=c++17'])

subdir('src')

s2c_proj = subproject('schema2cpp')
schema2cpp = find_program('schema2cpp')

input_config_hpp = custom_target(
    'input_config_hpp',
    input : [files('input.schema.json'), files('input.json')],
    output : 'InputConfig.hpp',
    command : [schema2cpp, '@INPUT0@', '--data', '@INPUT1@', '-o', '@OUTPUT@',
               '--name', 'InputConfig', '--data-name', 'defaultInputConfig'],
)

s2c_headers_dep = s2c_proj.get_variable('s2c_headers_dep')

secretGiver = executable('SecretGiver', sources + [input_config_hpp],
                         dependencies: s2c_headers_dep,
                         install: true)