//precision highp float;   

vec2 v;		// = vec2(1.0,  2.0);
int y;	// =  5;

bool test(){
	  bool checkpoint = true; // checkpoint should be true at all times
	  
	  switch(y){
	  	case 0: checkpoint = false;
	  	case 1: checkpoint = false;
	  	case 4: checkpoint = false;
	  	case 5: checkpoint = checkpoint && true; break;
	  	case 6: checkpoint = false;
	  }
	  
	   switch(y){
	  	case 0: checkpoint = false;
	  	case 1: checkpoint = false;
	  	case 6: checkpoint = false;
	  }
	  
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
