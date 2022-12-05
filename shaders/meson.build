
glsl_compiler = find_program('glslangValidator')
shaders = [
    'hello_triangle.frag',
    'hello_triangle.vert',
    'shader.frag',
    'shader.vert'
]
shader_targets = []
foreach s : shaders
    shader_targets += custom_target(
        s.split('.')[0] + '_' + s.split('.')[1] + '_shader',
        input : s,
        output : '@PLAINNAME@.spv',
        command : ['glslangValidator', '--target-env', 'vulkan1.2', '-o', '@OUTPUT@', '@INPUT@'],
        install : true,
        install_dir : 'shaders'
    )
endforeach