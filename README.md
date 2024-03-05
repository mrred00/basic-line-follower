### 1. Çizgi Takip Robotu Girişi:

Çizgi takip robotları, genellikle bir veya birden fazla çizgi izleme sensörü kullanarak zemindeki belirli bir çizgiyi takip etmek üzere tasarlanmış robotlardır. Bu sensörler, yüzeydeki renk farklılıklarını algılar ve bu bilgileri kullanarak robotun hareketini kontrol eder.

### 2. PID Kontrol:

PID (Proportional-Integral-Derivative) kontrol, bir sistemdeki hata sinyalini analiz ederek ve buna göre bir kontrol sinyali üreterek sistem davranışını düzeltmeye yönelik bir kontrol stratejisidir. PID kontrol algoritmaları, bir hedef değer ile gerçek değer arasındaki farkı kullanarak sistemi düzeltir.

### 3. Çizgi İzleme Algoritması:

Çizgi takip robotları, genellikle sensörlerle zeminden yansıyan ışığı ölçerek çizgiyi takip ederler. Bu sensörlerden gelen bilgileri kullanarak, robotun çizgi üzerinde hareket etmesini sağlamak için PID kontrol algoritması kullanılır.

### 4. Temel Matematiksel Hesaplar:

#### 4.1. Hata Hesabı:
Hata (`hata`), sensör değerinin beklenen değerden ne kadar sapma gösterdiğini belirten bir değerdir. Bu, çizgi izleme sensörlerinden gelen okuma değeri ile orta değer (örneğin, 1500) arasındaki farkı temsil eder.

```cpp
hata = qtra.readLine(sensorValues, 1, zemin) - 1500;
```

#### 4.2. PID Kontrolü:
PID kontrolü, hata değerine ve hata değişiminin zaman türevine dayanır. Kp, Kd ve Kp parametreleri, bu değerleri nasıl kullanacağımızı belirler.

```cpp
int duzeltmehizi = Kp * hata + Kd * (hata - sonhata);
```

#### 4.3. Motor Hızları:
Elde edilen düzeltilmiş hata (`duzeltmehizi`), taban hız değeri ile kullanılarak sağ ve sol motor hızları hesaplanır.

```cpp
solmotorpwm = tabanhiz + duzeltmehizi;
sagmotorpwm = tabanhiz - duzeltmehizi;
```

### 5. Motor Kontrol Fonksiyonu:

Motor kontrol fonksiyonu, sağ ve sol motorların PWM sinyallerini uygun şekilde ayarlar ve aynı zamanda hareket yönünü belirler.

```cpp
void motorkontrol(int sagmotorpwm, int solmotorpwm) {
  // Sağ ve sol motorların hız ve yön kontrolü
  // Yön bilgisi Serial Monitor'a yazdırılır
}
```

### 6. PID Kontrol Algoritması:

```cpp
int duzeltmehizi = Kp * hata + Kd * (hata - sonhata);
sonhata = hata;
solmotorpwm = tabanhiz + duzeltmehizi;
sagmotorpwm = tabanhiz - duzeltmehizi;
solmotorpwm = constrain(solmotorpwm, -255, maxhiz);
sagmotorpwm = constrain(sagmotorpwm, -255, maxhiz);
```

- `Kp` (Proportional Gain): Hata ile orantılı bir düzeltme sağlar. Yani, eğer robot çizgiden sapıyorsa, bu orantısal düzeltme miktarı artar.
  
- `Kd` (Derivative Gain): Hatanın zaman içindeki değişimini kontrol eder. Bu, robotun hızlı bir şekilde düzeltilmesine yardımcı olur.

- `duzeltmehizi`: PID kontrol algoritmasının çıkışıdır. Hatanın Kp ve Kd ile çarpılmasıyla elde edilir.

- `sonhata`: Bir önceki ölçülen hata değerini saklar.

- `solmotorpwm` ve `sagmotorpwm`: PID kontrolü ile düzeltilmiş hız değerleridir.

- `constrain()`: Bu fonksiyon, belirtilen aralıkta bir değer sağlar. Motor hızları, -255 ile +255 arasında olmalıdır.

### 7. Motor Kontrol Fonksiyonu:

```cpp
void motorkontrol(int sagmotorpwm, int solmotorpwm) {
  if (sagmotorpwm <= 0) {
    analogWrite(sagmotor1, 0);
    analogWrite(sagmotor2, abs(sagmotorpwm));
  } else {
    analogWrite(sagmotor1, sagmotorpwm);
    analogWrite(sagmotor2, 0);
  }

  if (solmotorpwm <= 0) {
    analogWrite(solmotor1, abs(solmotorpwm));
    analogWrite(solmotor2, 0);
  } else {
    analogWrite(solmotor1, 0);
    analogWrite(solmotor2, solmotorpwm);
  }

  // Yön bilgisi Serial Monitor'a yazdırılır
  if (sagmotorpwm > 0 && solmotorpwm > 0) {
    Serial.println("Forward");
  } else if (sagmotorpwm < 0 && solmotorpwm < 0) {
    Serial.println("Backward");
  } else if (sagmotorpwm > 0 && solmotorpwm == 0) {
    Serial.println("Right");
  } else if (sagmotorpwm == 0 && solmotorpwm > 0) {
    Serial.println("Left");
  }
}
```

Bu fonksiyon, sağ ve sol motorların PWM değerlerine göre kontrol edilmesini sağlar. Ayrıca, motorların hareket yönü bilgisi Serial Monitor'a yazdırılır.

### 8. Ana `loop()` Fonksiyonu:

```cpp
void loop() {
  hata = qtra.readLine(sensorValues, 1, zemin) - 1500;
  int duzeltmehizi = Kp * hata + Kd * (hata - sonhata);
  sonhata = hata;
  solmotorpwm = tabanhiz + duzeltmehizi;
  sagmotorpwm = tabanhiz - duzeltmehizi;
  solmotorpwm = constrain(solmotorpwm, -255, maxhiz);
  sagmotorpwm = constrain(sagmotorpwm, -255, maxhiz);
  motorkontrol(sagmotorpwm, solmotorpwm);
}
```

### 9. Çizgi İzleme Algoritması:

Çizgi izleme sensörleri, genellikle bir dizi fotosel veya IR sensörü kullanarak zemindeki çizgiyi algılarlar. Sensörlerin her biri, çizgi üzerinde veya çizgi dışında olma durumuna göre farklı değerler üretir.

### 10. PID Kontrol Algoritması:

PID kontrol algoritması, robotun çizgide kalmak için çizgi izleme sensörlerinden alınan bilgileri kullanır. Bu algoritma, hata (istenilen pozisyon ile sensör çıktısı arasındaki fark), hata değişimi (hata değerinin zaman içindeki değişimi) ve integral (hata değerlerinin toplamı) terimlerini kullanarak motor hızlarını ayarlar.

```cpp
int hata = (qtra.readLine(sensorValues, 1, zemin) - 1500);
int duzeltmehizi = Kp * hata + Kd * (hata - sonhata);
sonhata = hata;
solmotorpwm = tabanhiz + duzeltmehizi;
sagmotorpwm = tabanhiz - duzeltmehizi;
solmotorpwm = constrain(solmotorpwm, -255, maxhiz);
sagmotorpwm = constrain(sagmotorpwm, -255, maxhiz);
```

- `hata`: Çizgi izleme sensörlerinden alınan okuma değeri ile beklenen değer arasındaki farkı temsil eder.

- `duzeltmehizi`: PID kontrol algoritmasının çıkışıdır. Kp ve Kd terimleri ile hata değeri üzerinden hesaplanır.

- `sonhata`: Bir önceki ölçülen hata değerini saklar.

- `solmotorpwm` ve `sagmotorpwm`: PID kontrolü ile düzeltilmiş hız değerleridir.

### 11. Motor Kontrol Fonksiyonu:

Motor kontrol fonksiyonu, hesaplanan motor hızlarına göre motorları kontrol eder.

```cpp
void motorkontrol(int sagmotorpwm, int solmotorpwm) {
  if (sagmotorpwm <= 0) {
    analogWrite(sagmotor1, 0);
    analogWrite(sagmotor2, abs(sagmotorpwm));
  } else {
    analogWrite(sagmotor1, sagmotorpwm);
    analogWrite(sagmotor2, 0);
  }

  if (solmotorpwm <= 0) {
    analogWrite(solmotor1, abs(solmotorpwm));
    analogWrite(solmotor2, 0);
  } else {
    analogWrite(solmotor1, 0);
    analogWrite(solmotor2, solmotorpwm);
  }

  // Yön bilgisi Serial Monitor'a yazdırılır
  if (sagmotorpwm > 0 && solmotorpwm > 0) {
    Serial.println("Forward");
  } else if (sagmotorpwm < 0 && solmotorpwm < 0) {
    Serial.println("Backward");
  } else if (sagmotorpwm > 0 && solmotorpwm == 0) {
    Serial.println("Right");
  } else if (sagmotorpwm == 0 && solmotorpwm > 0) {
    Serial.println("Left");
  }
}
```

Bu fonksiyon, hesaplanan motor hızlarına göre sağ ve sol motorları kontrol eder. Ayrıca, motorların hareket yönü bilgisi Serial Monitor'a yazdırılır.

### 12. Ana `setup()` Fonksiyonu:

```cpp
void setup() {
  pinMode(solmotor1, OUTPUT);
  pinMode(solmotor2, OUTPUT);
  pinMode(sagmotor1, OUTPUT);
  pinMode(sagmotor2, OUTPUT);
  tone(led_buzzer, 500, 100);
  delay(50);
  noTone(led_buzzer);
  Serial.begin(9600);

  for (i = 0; i < 80; i++) {
    // Robot belirli yönlere hareket eder
    // Sensör kalibrasyonu yapılır
    qtra.calibrate();
    delay(3);
  }

  motorkontrol(0, 0);
  tone(led_buzzer, 3000, 100);
  delay(50);
  noTone(led_buzzer);
}
```

Çizgi takip robotunuzun kodu üzerindeki detaylı açıklamaları incelediğiniz için teşekkür ederim. Eğer daha fazla yardıma ihtiyacınız olursa veya başka konulara odaklanmak istiyorsanız, lütfen bana bildirin. Başarılar dilerim ve keyifli kodlamalar!

---

Mr.red

Embded System Developer & Android Front End Developer
