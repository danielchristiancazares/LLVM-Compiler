//precision highp float;   

vec2 v;		// = vec2(1.0,  2.0);
int y;	// =  3.0;

int test(){
	switch(y){
		case 1:
		case 2: return 2;
		case 3: return 3;
		default: return -1;
	}
}
// Expected output: 2

/*
void main()
{
  if(test() == 14911715) // Assert function returns expected value
    gl_FragColor = vec4(0,1,0,1.0); // green
  else
    gl_FragColor = vec4(1,0,0,1.0);  // red
}
*/
