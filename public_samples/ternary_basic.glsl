//precision highp float;   

bool test(){
  bool checkpoint = true;
  
  int x = true ? 1 : 0;
  int y = false? 1 : 0;
  
  checkpoint = x == 1 && checkpoint && y == 0;
  
  checkpoint = checkpoint ? true : false;
  
  float f = 1.0;
  f = true ? 2.0 : 3.0;
  checkpoint = checkpoint && f == 2.0;
  
  
  return checkpoint; 
}
// Expected output: -1

/*
void main()
{
  if(test()) // Assert function returns expected value
    gl_FragColor = vec4(0,1,0,1.0); // green
  else
    gl_FragColor = vec4(1,0,0,1.0);  // red
}
*/
