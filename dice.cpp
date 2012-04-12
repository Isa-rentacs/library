class DICE{
  /*
   * top = 0, right = 1, up = 2, left = 3, down = 4, bottom = 5; 
   */
public:
  DICE(int a, int b, int c, int d, int e, int f){
    v[0] = a;
    v[1] = b;
    v[2] = c;
    v[3] = d;
    v[4] = e;
    v[5] = f;
  }

  DICE(int a, int b, int c){
    v[0] = a;
    v[1] = b;
    v[2] = c;
    v[3] = 7-b;
    v[4] = 7-c;
    v[5] = 7-a;
  }
  int v[6];

  void rotate(int arg){
    int tmp=v[0];
    if(arg == 0){
      //North
      v[0] = v[4];
      v[4] = v[5];
      v[5] = v[2];
      v[2] = tmp;
    }else if(arg == 1){
      //East
      v[0] = v[3];
      v[3] = v[5];
      v[5] = v[1];
      v[1] = tmp;
    }else if(arg == 2){
      //West
      v[0] = v[1];
      v[1] = v[5];
      v[5] = v[3];
      v[3] = tmp;
    }else if(arg == 3){
      //South
      v[0] = v[2];
      v[2] = v[5];
      v[5] = v[4];
      v[4] = tmp;
    }else if(arg == 4){
      //right
      tmp = v[4];
      v[4] = v[1];
      v[1] = v[2];
      v[2] = v[3];
      v[3] = tmp;
    }else{
      tmp = v[4];
      v[4] = v[3];
      v[3] = v[2];
      v[2] = v[1];
      v[1] = tmp;
    }
  }

  int top(void){
    return v[0];
  }

  int bottom(void){
    return v[5];
  }
};
