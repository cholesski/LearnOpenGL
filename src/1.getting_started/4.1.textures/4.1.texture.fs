#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord); //procita konkretnu 
	//boju i postavi na izlaz fragentSh i onda on oboji tom bojom 
	//taj konkretan fragment
}
