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

tcp = library('base', 'tcp/tcp_server.cpp', dependencies: deps)

executable(
    'block-server',
    'block_server_run.cpp',
    install: true,
    dependencies: deps,
    link_args: ['-static'],
    link_with: [
        tcp,
    ],
)

executable(
    'client-emulator',
    'client_emulator.cpp',
    install: true,
    dependencies: deps,
    link_args: ['-static'],
)




