#define samp_siz 4 // количество расчетов - можно увеличить до 20
#define rise_threshold 5

void setup() {
    Serial.begin(115200);
    pinMode(A1, INPUT);
    pinMode(A1, OUTPUT);
}

void loop() {

  float reads[samp_siz], sum;
  long int now, ptr;
  float last, reader, start;
  float first, second, third, before, print_value;
  bool rising;
  int rise_count;
  int n;
  long int last_beat;

  for (int i=0; i<samp_siz; i++)
    reads[i] = 0;
  sum = 0;
  ptr = 0;

  // в течение 20 мс вычисляем среднее значение датчика
  while (1) {
    n = 0;
    start = millis();
    reader = 0.;
    do {
      reader += analogRead(A1);
      n++;
      now = millis();
    }

    // записываем полученное среднее значение в массив
    while (now < start + 20);
    reader /= n;  // we got an average
    sum -= reads[ptr];
    sum += reader;
    reads[ptr] = reader;
    last = sum / samp_siz;

    // проверяем, появляется ли увеличение среднего значения
    if (last > before) {
      rise_count++;

      // если значение увеличивается, что подразумевает сердцебиение
      // включаем светодиод и записываем время с последнего удара
      // отслеживаем два предыдущих удара, чтобы получить взвешенное среднее
      if (!rising && rise_count > rise_threshold) {
        digitalWrite(10, HIGH);
        rising = true;
        first = millis() - last_beat;
        last_beat = millis();

        // рассчитываем средневзвешенную частоту сердечных сокращений
        print_value = 60000. / (0.4 * first + 0.3 * second + 0.3 * third);
        Serial.println(print_value);
        third = second;
        second = first;
      }
    }
    else {
      rising = false;
      rise_count = 0;
      digitalWrite(10, LOW);
    }
    before = last;
    ptr++;
    ptr %= samp_siz;
  }
}
