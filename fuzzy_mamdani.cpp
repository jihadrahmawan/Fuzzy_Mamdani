//fungsi keanggotaan
float fukering[4] = {0, 0, 18, 36};
float funormal[3] = {18, 45, 72};
float fubasah[4] = {54, 72, 90, 90};

float pwmlambat[3] = {0, 0, 100};
float pwmsedang[3] = {20, 127.5, 235};
float pwmcepat[3] = {155, 255, 255};

float inputf, outputf;

float A, B;
int sel_;
float kering, normal_kering, normal_basah, basah;
float A1, A2, A3, A4, A5, A6, A7;
float M1, M2, M3, M4, M5, M6, M7;

float FiN(){
    if (inputf < fukering[2]){return 1;}
    else if (inputf >= fukering[2] && inputf <= fukering[3]){return (fukering[3] - inputf)/(fukering[3]-fukering[2]);}
    else if (inputf > fukering[3]){return 0;}
}
float FiZ(){
    if (inputf < funormal[0]){return 0;}
    else if (inputf >= funormal[0] && inputf <= funormal[1]){return (inputf - funormal[0])/(funormal[1]-funormal[0]);}
    else if (inputf >= funormal[1] && inputf <= funormal[2]){return (funormal[2] - inputf)/(funormal[2]-funormal[1]);}
    else if (inputf > funormal[2]){return 0;}
}
float FiP(){
    if (inputf < fubasah[0]){return 0;}
    else if (inputf >= fubasah[0] && inputf <= fubasah[1]){return (inputf - fubasah[0])/(fubasah[1]-fubasah[0]);}
    else if (inputf > fubasah[2]){return 1;}
}
float FoN(){
    if (outputf < pwmlambat[1]){return 1;}
    else if (outputf >= pwmlambat[1] && outputf <= pwmlambat[2]){return (pwmlambat[2] - outputf)/(pwmlambat[2]-pwmlambat[1]);}
    else if (outputf > pwmlambat[2]){return 0;}
}
float FoZ(){
    if (outputf < pwmsedang[0]){return 0;}
    else if (outputf >= pwmsedang[0] && outputf <= pwmsedang[1]){return (outputf - pwmsedang[0])/(pwmsedang[1]-pwmsedang[0]);}
    else if (outputf >= pwmsedang[1] && outputf <= pwmsedang[2]){return (pwmsedang[2] - outputf)/(pwmsedang[2]-pwmsedang[1]);}
    else if (outputf > pwmsedang[2]){return 0;}
}
float FoP(){
    if (outputf < pwmcepat[0]){return 0;}
    else if (outputf >= pwmcepat[0] && outputf <= pwmcepat[1]){return (outputf - pwmcepat[0])/(pwmcepat[1]-pwmcepat[0]);}
    else if (outputf > pwmcepat[1]){return 1;}
}

void implikasi (){
//sesuai dengan rule
// if deket then lambat
kering = 100 - (FiN() * (pwmlambat[2] - pwmlambat[1]));
// if lumayan then sedang
normal_kering = 20 + (FiZ() * (pwmsedang[1] - pwmsedang[0]));
normal_basah =  235 - (FiZ() * (pwmsedang[2] - pwmsedang[1]));
// if jauh then cepat
basah = 155 + (FiP() * (pwmcepat[1] - pwmcepat[0]));
}

/*Function definition to perform integration 1/3rd Rule */
float simpsons(float f(float x), float a,float b,float n){
  float h,integral,x,sum=0;
  int i;
  h=fabs(b-a)/n;
  for(i=1;i<n;i++){
    x=a+i*h;
    if(i%2==0){
      sum=sum+2*f(x);
    }
    else{
      sum=sum+4*f(x);
    }
  }
  integral=(h/3)*(f(a)+f(b)+sum);
  return integral;
}
float fx(float limd, float limu, float a, float b, int sel){
    int n,i=2;
    float h,x,integral,eps=0.1,integral_new;
    A = a;
    B = b;
    sel_ = sel;
    integral_new=simpsons(f,limd,limu,i);
    do{
        integral=integral_new;
        i=i+2;
        integral_new=simpsons(f,limd,limu,i);
    }while(fabs(integral_new-integral)>=eps);
    
    return integral_new;
}

void luas_deffuzzy(){
implikasi ();
kering = ((pwmlambat[2] - kering) * FiN()) / 2; //= 15.432222
A2 = (kering - pwmlambat[0]) * FiN(); // = 24.69155
A3 = ((normal_kering - pwmsedang[0])* FiZ()) / 2; // = 0
A4 = ((pwmsedang[2] - normal_basah)* FiZ()) / 2; // = 0
A5 = (normal_basah - normal_kering)*FiZ(); // = 0
A6 = ((basah - pwmcepat[0]) * FiP()) / 2; // = 0
A7 = (pwmcepat[2] - basah)* FiP(); // = 0
}
float f(float x){
    if (B > 0 && sel_ == 0){
    return ((x-A)/B)*x;
    }
    else if (B > 0 && sel_ == 1){
    return ((A-x)/B)*x;
    }
    else {
    return A*x;
    }
}

void moment(){
luas_deffuzzy();
//M1 = ∫ ((100-x)/100)x dx ==================> limd kering dan limup 100
M1 = fx(kering, pwmlambat[2], pwmlambat[2], (pwmlambat[2]-pwmlambat[0]), 1);
//M2 = ∫ 0.555556x dx ==================> limd 0 dan limup kering
M2 = fx(pwmlambat[0], kering, FiN(), 0, 0);
//M3 = ∫ ((x-20)/107.5)x dx ==================> limd 20 dan limup normal_kering
M3 = fx(pwmsedang[0], normal_kering, pwmsedang[0], (pwmsedang[1] - pwmsedang[0]), 0);
//M4 = ∫ ((235-x)/107.5)x dx ==================> limd normal_basah dan limup 235
M4 = fx (normal_basah, pwmsedang[2], pwmsedang[2], (pwmsedang[2]-pwmsedang[1]), 1);
//M5 = ∫ 0 dx ==================> limd normal_kering dan limup normal_basah
M5 = fx (normal_kering, normal_basah, FiZ(), 0, 0);
//M6 = ∫ ((x-155)/100)x dx ==================> limd 155 dan limup basah
M6 = fx(pwmcepat[0], basah, pwmcepat[0], (pwmcepat[2]-pwmcepat[0]), 0);
//M7 = ∫ 0 dx ==================> limd basah dan limup 255
M7 = fx(basah, pwmcepat[2], FiP(), 0, 0);
}
float deffuzzyfikasi(){
return (M1+M2+M3+M4+M5+M6+M7)/(kering+A2+A3+A4+A5+A6+A7);
}

void setup() {
  // put your setup code here, to run once:

}
float nilai_sensor = 0.0;
float pwm=0.0;
void loop() {
  inputf = nilai_sensor; // 0-90%
  moment();
  pwm = deffuzzyfikasi();
  // put your main code here, to run repeatedly:

}
