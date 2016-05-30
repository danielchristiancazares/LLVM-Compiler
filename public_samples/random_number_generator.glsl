//precision highp float;   

vec2 v;		// = vec2(1.0,  2.0);
float y;	// =  3.0;


int entropy = 1;
int mod(int x, int m){
	return x - (x/m)*m;
}


int rand(){
	return entropy = mod(11 * entropy + 37, 100);
}


bool test(){
	int i;
	for(i = 0; i < 100; ++i){
		
	
	}
	return rand() == 48 && rand() == 65 && rand() == 52 && rand() == 9 && rand() == 36;
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
