project(
    'baum-web',
    'cpp',
    version: '0.1',
    default_options: [
        'warning_level=3',
        'cpp_std=c++20',
        'default_library=static',
    ],
)

deps = [dependency('boost')]


executable(
    'block-server',
    'block_server.cpp',
    install: true,
    dependencies: deps,
    link_args: ['-static'],

)

executable(
    'client-emulator',
    'client_emulator.cpp',
    install: true,
    dependencies: deps,
    link_args: ['-static'],
)




