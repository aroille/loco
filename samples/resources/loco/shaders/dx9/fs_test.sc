$input v_texcoord0 // in...

/*
 * Copyright 2011-2014 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include "../common/common.sh"

SAMPLER2D(u_texColor, 0);

void main()
{
	//gl_FragColor = texture2D(u_texColor, v_texcoord0);
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//gl_FragColor.w = 1.0;
}
