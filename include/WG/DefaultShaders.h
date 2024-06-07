#ifndef DEFAULT_SHADERS_H_
#define DEFAULT_SHADERS_H_

#define TEXTURE_VERTEX_SHADER_SOURCE \
"#version 460 core\n\
\
layout (location = 0) in vec2 vertex_position;\n\
layout (location = 1) in vec2 vertex_texture_coord;\n\
layout (location = 2) in vec4 vertex_color;\n\
\
uniform mat4 projection;\n\
\
out vec2 texture_coord;\n\
out vec4 color;\n\
\
void main(){\n\
    gl_Position = projection * vec4(vertex_position,0.0,1.0);\n\
    texture_coord = vertex_texture_coord;\n\
    color = vertex_color;\n\
}" 

#define TEXTURE_FRAGMENT_SHADER_SOURCE \
"#version 460 core\n\
\n\
in vec2 texture_coord;\n\
in vec4 color;\n\
\n\
out vec4 fragment_color;\n\
\n\
uniform int texture_format;\n\
uniform sampler2D texture_sampler;\n\
\n\
void main(){\n\
\n\
    vec4 texture_color = texture(texture_sampler,texture_coord);\n\
    fragment_color = texture_color * color;\n\
}"


#define SHAPE_VERTEX_SHADER_SOURCE \
"#version 460 core\n\
\
layout (location = 0) in vec2 vertex_position;\n\
layout (location = 1) in vec4 vertex_color;\n\
\
out vec4 color;\n\
\
uniform mat4 projection;\n\
\
void main(){\n\
    gl_Position = projection * vec4(vertex_position,0.0,1.0);\n\
    color = vertex_color;\n\
}"

#define SHAPE_FRAGMENT_SHADER_SOURCE \
"#version 460 core\n\
\
in vec4 color;\n\
out vec4 fragment_color;\n\
\
void main(){\n\
    fragment_color = color;\n\
}"


#endif