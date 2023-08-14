#version 330 core
out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main() {
	vec2 texSize = 1.0 / vec2(textureSize(texture1, 0));
	vec4 currPixel = texture(texture1, TexCoord);
	vec4 nextPixel = texture(texture1, TexCoord + vec2(texSize.x,0));
	vec4 downPixel = texture(texture1, TexCoord + vec2(0,texSize.y));
	vec4 crossPixel = texture(texture1, TexCoord + texSize);
	fragColor = (currPixel + nextPixel + downPixel + crossPixel)/4.0;
}
