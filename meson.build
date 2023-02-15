project(
    'riscv32',
    'c',
    license: '0BSD',
    version: '1.0.0',
    default_options: [
        'c_std=c11',
        'warning_level=3',
    ],
)

emu_exe = executable(
    'riscv32',
    files(
        'src/main.c',
        'src/riscv32.c',
    )
)
