$input v_texcoord0 // in...

#include "common.sh"

SAMPLER2D(u_texColor, 0);

void main()
{
	gl_FragColor = texture2D(u_texColor, v_texcoord0);
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//gl_FragColor.w = 1.0;
}

