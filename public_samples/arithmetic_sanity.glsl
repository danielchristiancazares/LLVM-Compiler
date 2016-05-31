//precision highp float;   

vec2 v2; // = vec2(1.0, 2.0);
vec3 v3; // = vec3(-1.0, 3.0, 5.0);
vec4 v4; // = vec4(0.0, 1.0, 2.0, 3.0);
float f; // = 2.5;
bool b;  // = true;



float hash(vec2 l2, vec3 l3, vec4 l4, float g, bool flag){
   float hash = l2.x;
   hash *= 7.0;
   hash += l2.y;
   hash *= 7.0;
   hash += l3.x;
   hash *= 7.0;
   hash += l3.y;
   hash *= 7.0;
   hash += l3.z;
   hash *= 7.0;
   hash += l4.x;
   hash *= 7.0;
   hash += l4.y;
   hash *= 7.0;
   hash += l4.z;
   hash *= 7.0;
   hash += l4.w;
   
   
   hash *= 7.0;
   hash += g;
   
   if(flag) hash *= 2.0;
   
   return hash;
}


float test(){ 
  v2 = v2 + (1.0 + 0.0);
  v3 = v3 + (1.0 - 0.0); 
  v4 = v4 + (1.0 * 1.0); 
  v2 = 2.0 + v2;
  v3 = 2.0 + v3; 
  v4 = 2.0 + v4;
  
  
  v2 = v2 - (3.0 / 1.0);
  v3 = v3 - 3.0; 
  v4 = v4 - 3.0; 
  v2 = 4.0 - v2;
  v3 = 4.0 - v3; 
  v4 = 4.0 - v4;
  
  
  v2 = v2 * 5.0;
  v3 = v3 * 5.0; 
  v4 = v4 * 5.0; 
  v2 = 6.0 * v2;
  v3 = 6.0 * v3; 
  v4 = 6.0 * v4;
  
  v2 = v2 / 7.0;
  v3 = v3 / 7.0; 
  v4 = v4 / 7.0; 
  v2 = 8.0 / v2;
  v3 = 8.0 / v3; 
  v4 = 8.0 / v4;
  

  v2 = (v2 = v2);
  v3 = v3 = v3*2.0;
  
  v2 = (v2 -= f);
  v3 = (v3 -= f);
  v4 = (v4 -= f);
  
  v2 = (v2 *= f);
  v3 = (v3 *= f);
  v4 = (v4 *= f);
  
  v2 = (v2 /= f);
  v3 = (v3 /= f);
  v4 = (v4 /= f);
  
  v2 = (v2 += f);
  v3 = (v3 += f);
  v4 = (v4 += f);
  
  b = b && (b || b);
  
  
  v2.x += v2.y;
  v3.z = v2.x;
  v4.xywz = (v2.xyxy / v3.yxyx - v4.wzyx).xyzw;
  v2.y -= v3.z + 1.0;
  v4.x = 0.5;
  v2.xy /= 2.0;
  
  return hash(v2, v3, v4, f, b);
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
