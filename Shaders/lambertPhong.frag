varying vec4 cLightDiff, cLightAmb, cLightSpec;
varying vec4 cMatDiff, cMatAmb, cMatSpec;
varying vec3 camDirection;
varying vec3 N;
varying vec3 L;

uniform float powSpec;

void main (void)  
{     
   vec4 cFinal = vec4(0.0,0.0,0.0,1.0);
   float iDiff, iSpec;
   vec3 vRef;

   //Componente Especular Phong
   vRef = -normalize(reflect(L.xyz, N));
   iSpec = pow(max(dot(vRef, normalize(camDirection)),0.0),powSpec);

   //Componente difuso
   iDiff = max(dot(normalize(N), normalize(L.xyz)), 0.0);

   cFinal = cLightAmb*cMatAmb + iDiff*(cLightDiff*cMatDiff) + iSpec*(cLightSpec + cMatSpec);
   cFinal.w = 1.0;

   //gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);  
   gl_FragColor = cFinal;
}    
