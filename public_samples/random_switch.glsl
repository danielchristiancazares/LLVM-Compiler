//precision highp float;   

int entropy = 1;

int mod(int x, int m){
	return x - (x/m)*m;
}

int rand(int max){
	return mod(entropy = mod(11 * entropy + 37, 100), max);
}




bool test(){
	int s1 = 1;
	bool checkpoint = true;  // Checkpoint will always be true

	int i;
	for(i = 0; i < 25; ++i){
		switch(rand(5)){
			case 1: ++s1;
			case 100:
			case 200:
			case 2: s1 *= 2; break;
			case 4: 
			default: s1 + 3; 
			case 3: s1 += 2; break;
		}
	} 

	checkpoint = checkpoint && s1 == 8526;

	s1 = 1;
	for(i = 0; i < 25; ++i){
		switch(rand(7)){
			case 1: s1 += 2; 
			case 2: s1 *= 2; break;
			case 3: {s1 += 1; break;}
			case 4: {break;}
			case 5: s1++;
			case 6: s1 *= 3; 
		}
	}

	checkpoint = checkpoint && s1 == 379810;
	return checkpoint;
}

// Expected output: -1

/*
void main()
{
  if(test() == 14911715) // Assert function returns expected value
    gl_FragColor = vec4(0,1,0,1.0); // green
  else
    gl_FragColor = vec4(1,0,0,1.0);  // red
}
*/
