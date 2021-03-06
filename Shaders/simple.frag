//#version 130

uniform bool picking;
uniform float ID;

varying vec4 smoothColor;
varying vec4 surfaceNormal;
varying vec4 lightVector;
varying vec4 viewVector;

void main()
{
	vec4 aColor = 0.15*smoothColor;
	vec4 dColor = clamp(dot(lightVector, surfaceNormal), 0.0, 1.0)*smoothColor;
	vec4 sColor = pow(clamp(dot(viewVector,reflect(-lightVector,surfaceNormal)), 0.0, 1.0),1.0)*vec4(1,1,1,1);
	//gl_FragColor = surfaceNormal;
	
	//gl_FragColor = smoothColor;
	
	if(picking){
		gl_FragColor = vec4(ID/255.0, ID/255.0, ID/255.0, 1.0);
	} else {
		gl_FragColor = aColor + dColor + sColor;
	}
}
