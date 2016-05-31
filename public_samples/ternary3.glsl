//precision highp float;   

bool bar(){
	if(false) return false;
	else if (true) return true;
	else return false;
}


bool foo(){
	if(true? false : true) return true;
	else if (true) return bar() ? false : true;
	else return false;
}

bool test(){
	return true ? (false? test() : foo()) : bar();
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
