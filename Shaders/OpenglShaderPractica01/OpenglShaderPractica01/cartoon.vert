varying vec4 cFinal;
varying vec4 cMatDiff, cMatAmb, cMatSpec, cMatEmit;

varying vec3 N;
varying vec4 L;

void main(void){

	vec4 V;

	V = gl_ModelViewMatrix * gl_Vertex;
	N = normalize(gl_NormalMatrix * gl_Normal);
	L = normalize(gl_LightSource[0].position - V);

	cMatDiff = gl_FrontMaterial.diffuse;
	cMatAmb = gl_FrontMaterial.ambient;
	cMatSpec = gl_FrontMaterial.specular;

	float iDiff;
	iDiff = max(dot(normalize(N),normalize(L.xyz)), 0.0) ;

	if (iDiff < 0.2){
		cFinal = cMatAmb;

	}else{
		
		if (iDiff < 0.65)
			cFinal = cMatSpec;
		else
			cFinal = cMatDiff;
		
	}

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}