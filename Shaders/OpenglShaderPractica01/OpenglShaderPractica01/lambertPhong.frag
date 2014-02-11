varying vec4 cLightDiff, cLightAmb, cLightSpec;
varying vec4 cMatDiff, cMatAmb, cMatSpec;
varying vec3 camDirection;
varying vec3 N;
varying vec4 L;


uniform float bias;
uniform float roughness;
uniform float sharpness;
uniform float specular;
uniform float eta;
uniform float Kfr;
uniform float fresnel;

float biasFunc(float t, float a) {
	return pow(t,-(log(a)/log(2)));
}

float bdiff() {
	float C = 0.0;
	vec3 Ln =normalize(L.xyz);
	vec3 Nn = normalize(N);
	float LnxNn = dot(Nn, Ln);
	
	C = biasFunc(max(LnxNn, 0.0), bias);
	return C;
}


float Glossy(vec3 vRef) {
	float C = 0.0;
	float w = 0.18 * (1.0-sharpness);
	C = smoothstep(0.72 - w, 0.72 + w, pow(max(0.0,dot(N,vRef)), 1.0/roughness));
	return C;
}


float FresnelSchlickFunc() {
	
	vec3 Nn = normalize(N);
	vec3 Vn = -normalize(camDirection);
	float dotnv = abs(dot(Nn, Vn));
	float Kr = eta + (1.0-eta)*pow(1.0-dotnv,5.0);
	Kr = Kfr*biasFunc(Kr,bias);
	return Kr;

} 


void main (void)  
{     
   vec4 cFinal = vec4(0.0,0.0,0.0,1.0);
   float iDiff, iSpec;
   vec3 vRef;

   vRef = -normalize(reflect(L.xyz,N));
   // Componente difuso
   if (fresnel == 1.0) {
		iDiff = FresnelSchlickFunc();
		iSpec = 0.0;
   } else {
		iDiff = bdiff();
		// Componente Specular
		if (specular == 1.0) {
			iSpec = Glossy(vRef);
		 } else {
			iSpec = 0.0;
		}
   }

	cFinal = cLightAmb*cMatAmb + iDiff*(cLightDiff*cMatDiff) + iSpec*(cLightSpec*cMatSpec);
	cFinal.w = 1.0;

   gl_FragColor = cFinal;

}    
