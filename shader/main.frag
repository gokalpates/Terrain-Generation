#version 460 core

out vec4 fragColor;

in float height;

void main()
{
	if(height < 30)
		fragColor = vec4(0.016,0.318,0.58,1.f);
	else if(height < 50)
		fragColor = vec4(0.243,0.4,0.008,1.f);
	else if(height < 70)
		fragColor = vec4(0.4,0.224,0.008,1.f);
	else if(height < 90)
		fragColor = vec4(0.553,0.58,0.553,1.f);
	else if(height < 100)
		fragColor = vec4(1.f,1.f,1.f,1.f);

	fragColor = vec4(pow(fragColor.xyz,vec3(1.f/2.2f)), 1.0);
}