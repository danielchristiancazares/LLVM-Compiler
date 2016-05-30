//precision highp float;   

mat3 m3;
vec3 r1;
vec3 r2;
vec3 r3;

bool test(){
	m3[0] = r1;
	m3[1] = r2;
	m3[2] = r3;
	
	
	
	m3 *= 2.0;
	bool checkpoint = m3[0][0] == 2.0 && m3[1][1] == 10.0 && m3[2][1] == 16.0;
	
	
	++m3;
	checkpoint = checkpoint && m3[0][0] == 3.0 && m3[1][1] == 11.0 && m3[2][1] == 17.0;

	m3--;
	checkpoint = checkpoint && m3[0][0] == 2.0 && m3[1][1] == 10.0 && m3[2][1] == 16.0;
	
	m3 = -m3;
	checkpoint = checkpoint && m3[0][0] == -2.0 && m3[1][1] == -10.0 && m3[2][1] == -16.0;

	m3 = +m3;
	m3 = m3;
	checkpoint = checkpoint && m3[0][0] == -2.0 && m3[1][1] == -10.0 && m3[2][1] == -16.0;
	
	
	m3 = 1.0 - m3;
	checkpoint = checkpoint && m3[0][0] == 3.0 && m3[1][1] == 11.0 && m3[2][1] == 17.0;
	
	m3 = m3 - 1.0;
	checkpoint = checkpoint && m3[0][0] == 2.0 && m3[1][1] == 10.0 && m3[2][1] == 16.0;
	
	return checkpoint;
}
// Expected output: 3432355

/*
void main()
{
  if(test() == 14911715) // Assert function returns expected value
    gl_FragColor = vec4(0,1,0,1.0); // green
  else
    gl_FragColor = vec4(1,0,0,1.0);  // red
}
*/
