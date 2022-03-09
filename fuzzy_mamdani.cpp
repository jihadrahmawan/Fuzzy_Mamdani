#include<math.h>
float A, B;
int sel_;
float a1, b1a, b1b, c1;
float L1, L2, L3, L4, L5, L6, L7;
float M1, M2, M3, M4, M5, M6, M7;

float fukering[4] = {0, 0, 18, 36};
float funormal[3] = {18, 45, 72};
float fubasah[4] = {54, 72, 90, 90};

float pwmlambat[3] = {0, 0, 100};
float pwmsedang[3] = {20, 127.5, 235};
float pwmcepat[3] = {155, 255, 255};

float inputf, outputf;
float FiN() {
  if (inputf < fukering[2]) {
    return 1;
  }
  else if (inputf >= fukering[2] && inputf <= fukering[3]) {
    return (fukering[3] - inputf) / (fukering[3] - fukering[2]);
  }
  else if (inputf > fukering[3]) {
    return 0;
  }
}
float FiZ() {
  if (inputf < funormal[0]) {
    return 0;
  }
  else if (inputf >= funormal[0] && inputf <= funormal[1]) {
    return (inputf - funormal[0]) / (funormal[1] - funormal[0]);
  }
  else if (inputf >= funormal[1] && inputf <= funormal[2]) {
    return (funormal[2] - inputf) / (funormal[2] - funormal[1]);
  }
  else if (inputf > funormal[2]) {
    return 0;
  }
}
float FiP() {
  if (inputf < fubasah[0]) {
    return 0;
  }
  else if (inputf >= fubasah[0] && inputf <= fubasah[1]) {
    return (inputf - fubasah[0]) / (fubasah[1] - fubasah[0]);
  }
  else if (inputf > fubasah[1]) {
    return 1;
  }
}
float FoN() {
  if (outputf < pwmlambat[1]) {
    return 1;
  }
  else if (outputf >= pwmlambat[1] && outputf <= pwmlambat[2]) {
    return (pwmlambat[2] - outputf) / (pwmlambat[2] - pwmlambat[1]);
  }
  else if (outputf > pwmlambat[2]) {
    return 0;
  }
}
float FoZ() {
  if (outputf < pwmsedang[0]) {
    return 0;
  }
  else if (outputf >= pwmsedang[0] && outputf <= pwmsedang[1]) {
    return (outputf - pwmsedang[0]) / (pwmsedang[1] - pwmsedang[0]);
  }
  else if (outputf >= pwmsedang[1] && outputf <= pwmsedang[2]) {
    return (pwmsedang[2] - outputf) / (pwmsedang[2] - pwmsedang[1]);
  }
  else if (outputf > pwmsedang[2]) {
    return 0;
  }
}
float FoP() {
  if (outputf < pwmcepat[0]) {
    return 0;
  }
  else if (outputf >= pwmcepat[0] && outputf <= pwmcepat[1]) {
    return (outputf - pwmcepat[0]) / (pwmcepat[1] - pwmcepat[0]);
  }
  else if (outputf > pwmcepat[1]) {
    return 1;
  }
}
void implikasi () {
  a1 = 100 - (FiN() * (pwmlambat[2] - pwmlambat[1]));
  b1a = 20 + (FiZ() * (pwmsedang[1] - pwmsedang[0]));
  b1b =  235 - (FiZ() * (pwmsedang[2] - pwmsedang[1]));
  c1 = 155 + (FiP() * (pwmcepat[1] - pwmcepat[0]));
}
void luas_deffuzzy() {
  implikasi ();
  L1 = ((pwmlambat[2] - a1) * FiN()) / 2; 
  L2 = (a1 - pwmlambat[0]) * FiN(); 
  L3 = ((b1a - pwmsedang[0]) * FiZ()) / 2; 
  L4 = ((pwmsedang[2] - b1b) * FiZ()) / 2; 
  L5 = (b1b - b1a) * FiZ(); 
  L6 = ((c1 - pwmcepat[0]) * FiP()) / 2; 
  L7 = (pwmcepat[2] - c1) * FiP(); 
}
float f(float x) {
  if (B > 0 && sel_ == 0) {
    return ((x - A) / B) * x;
  }
  else if (B > 0 && sel_ == 1) {
    return ((A - x) / B) * x;
  }
  else {
    return A * x;
  }
}

float simpsons(float f(float x), float a, float b, float n) {
  float h, integral, x, sum = 0;
  int i;
  h = fabs(b - a) / n;
  for (i = 1; i < n; i++) {
    x = a + i * h;
    if (i % 2 == 0) {
      sum = sum + 2 * f(x);
    }
    else {
      sum = sum + 4 * f(x);
    }
  }
  integral = (h / 3) * (f(a) + f(b) + sum);
  return integral;
}
float fx(float limd, float limu, float a, float b, int sel) {
  int n, i = 2;
  float h, x, integral, eps = 0.1, integral_new;
  A = a;
  B = b;
  sel_ = sel;
  integral_new = simpsons(f, limd, limu, i);
  do {
    integral = integral_new;
    i = i + 2;
    integral_new = simpsons(f, limd, limu, i);
  } while (fabs(integral_new - integral) >= eps);
  return integral_new;
}

void moment() {
  luas_deffuzzy();
  M1 = fx(a1, pwmlambat[2], pwmlambat[2], (pwmlambat[2] - pwmlambat[0]), 1);
  M2 = fx(pwmlambat[0], a1, FiN(), 0, 0);
  M3 = fx(pwmsedang[0], b1a, pwmsedang[0], (pwmsedang[1] - pwmsedang[0]), 0);
  M4 = fx (b1b, pwmsedang[2], pwmsedang[2], (pwmsedang[2] - pwmsedang[1]), 1);
  M5 = fx (b1a, b1b, FiZ(), 0, 0);
  M6 = fx(pwmcepat[0], c1, pwmcepat[0], (pwmcepat[2] - pwmcepat[0]), 0);
  M7 = fx(c1, pwmcepat[2], FiP(), 0, 0);
}
float deffuzzyfikasi() {
  moment();
  return (M1 + M2 + M3 + M4 + M5 + M6 + M7) / (L1 + L2 + L3 + L4 + L5 + L6 + L7);
}
void setup() {
  Serial.begin(9600);

}
void loop() {
  if (Serial.available()) {
    int a = Serial.parseInt();
    if (a >= 0) {
      inputf = a;
      Serial.print("Input jarak :");
      Serial.print(inputf);
      Serial.print("Output Deffuzyfikasi Mamdani :");
      Serial.println(deffuzzyfikasi());
    }
  }
}
