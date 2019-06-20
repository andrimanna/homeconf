#define melodyPin 15
#define vel 0.8
#define ott 1.5

float melody0[] =
{
  262, 196, 220, 247, 165, 165, 220, 196, 175, 196, 131,
  131, 147, 147, 165, 175, 175, 196, 220, 247, 262, 294
};

float melody1[] =
{
  196, 330, 294, 262, 294, 247, 196, 262, 247, 220, 247,
  165, 165, 220, 196, 175, 196, 131, 131, 262, 247, 220, 196
};

float noteDurations0[] =
{
 832, 624, 208, 832, 416, 416, 832, 624, 208, 832, 416,
 416, 832, 624, 208, 832, 624, 208, 832, 416, 416, 1248
};

float noteDurations1[] =
{
  416, 832, 624, 208, 832, 416, 416, 832, 624, 208, 832,
  416, 416, 832, 624, 208, 832, 416, 416, 832, 624, 208, 416
};

void ring0() {
  for (unsigned int thisNote = 0; thisNote < sizeof(melody0) / sizeof(int); thisNote++)
  {    
    tone(melodyPin, melody0[thisNote] * ott, noteDurations0[thisNote] * vel * 0.7);    
    delay(noteDurations0[thisNote] * vel);    
    noTone(melodyPin);
  }
}

void ring1() {
  for (unsigned int thisNote = 0; thisNote < sizeof(melody1) / sizeof(int); thisNote++)
  {    
    tone(melodyPin, melody1[thisNote] * ott, noteDurations1[thisNote] * vel * 0.7);    
    delay(noteDurations1[thisNote] * vel);    
    noTone(melodyPin);
  }
}
