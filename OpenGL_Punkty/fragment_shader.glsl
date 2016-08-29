#version 450

in vec3 kolor;

out vec4 kolor_wyjsciowy;

void main()
{
	kolor_wyjsciowy = vec4(kolor, 1.0);
}