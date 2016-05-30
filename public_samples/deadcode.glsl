//precision highp float;   

int y; // = 1

int test(){
    int x = 0;
	while (y == 1){
		++y;
		continue;
		x += 1;
	}
	// y = 2

	while(y == 2){
		++y;
		break;
		x += 10;
	}
	// y = 3
	
	return y*1000 + x;  // Should be 3000
	++x;
	return -1; 
	
	if(true)
		return -2;
	else 
		return -3;
	return -4;
}
// Expected output: 3000

/*
void main()
{
  if(test() == 3000) // Assert function returns expected value
    gl_FragColor = vec4(0,1,0,1.0); // green
  else
    gl_FragColor = vec4(1,0,0,1.0);  // red
}
*/
