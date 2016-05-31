//precision highp float;   

int y;	// =  1;



int test2(){
	return y;
}


int test(int y /*5*/){
	return 100*y + test2();
}




// Expected output: 501

/*
void main()
{
  if(test() == 501) // Assert function returns expected value
    gl_FragColor = vec4(0,1,0,1.0); // green
  else
    gl_FragColor = vec4(1,0,0,1.0);  // red
}
*/
