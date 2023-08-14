#version 330 core
out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main() {
    fragColor = texture(texture1, TexCoord);
    //fragColor = vec4(1.0f,0.0f,0.0f,1.0f);
}
