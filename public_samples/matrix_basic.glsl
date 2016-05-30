mat2 m;
mat2 m2;


bool foo(){
  bool checkpoint = true;

  m[0][0] = 1.0;
  m[1][0] = 1.0;
  m[0][1] = 1.0;
  m[1][1] = 1.0;
  

  checkpoint = checkpoint && m[0][0] == 1.0 && m[1][1] == 1.0;
  
  
  m = m + m;
 
  checkpoint = checkpoint && m[0][0] == 2.0 && m[1][1] == 2.0;
  
  
  m2[0][0] = 1.0; 
  m2[0][1] = 2.0;
  m2[1][0] = 3.0;
  m2[1][1] = 4.0;
  
  m2 -= m; 
 
 
  checkpoint = checkpoint && m2[0][0] == 1.0 - 2.0;
  checkpoint = checkpoint && m2[0][1] == 2.0 - 2.0;
  checkpoint = checkpoint && m2[1][0] == 3.0 - 2.0;
  checkpoint = checkpoint && m2[1][1] == 4.0 - 2.0;
  
  return checkpoint;
}
