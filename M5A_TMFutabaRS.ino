/*-----------------------------------------------
 TMFutabaRS(Futaba RSサーボモータコマンド制御) 
 for M5Atom
 2021-05-31 t.hashimoto
 ------------------------------------------------*/
#include "TMFutabaRS.h"
#include <M5Atom.h>

// Ledの設定
#define ledGreen M5.dis.drawpix(0, 0xf00000);
#define ledRed M5.dis.drawpix(0, 0x00f000);
#define ledBlue M5.dis.drawpix(0, 0x0000f0);
#define ledWhite M5.dis.drawpix(0, 0x707070);

// Serial1
const int PinRX = 21; // From BTE094 TXD (Blue)
const int PinTX = 25; // From BTE094 RXD (Green)

// サーボの定義
TMFutabaRS servo1(1);
TMFutabaRS servo2(2);
TMFutabaRS servo3(3);
TMFutabaRS servo4(4);
TMFutabaRS servo5(5);


void setup() {
  Serial.begin(115200);
  M5.begin(true, false, true);
  delay(1000);

  // サーボ制御用
  Serial1.begin(115200, SERIAL_8N1, PinRX, PinTX);

  // サーボにサーボ制御用シリアルをセット
  servo1.begin(&Serial1);
  servo2.begin(&Serial1);
  servo3.begin(&Serial1);
  servo4.begin(&Serial1);
  servo5.begin(&Serial1);

  ledRed;

  // メモリマップの読み込み
  if (servo1.MemoryMap00to29()) {
    // 情報表示
    DispSerialMap00to29(&servo1);
  }
  else {
    // 失敗時
    Serial.println("Cannot read map 00-29");
  }

  if (servo1.MemoryMap30to59() == false) {
    // 情報表示
    DispSerialMap30to59(&servo1);
  }
  else {
    // 失敗時
    Serial.println("Cannot read memory map 30-59");
  }

  if (servo1.MemoryMap42to59()) {
    // 情報表示
    DispSerialMap42to59(&servo1);
  }
  else {
    // 失敗時
    Serial.println("Cannot read memory map 42-59");
  }

  // ボタンの何かを押すまで待機
  while (M5.Btn.wasPressed() == false) {
    M5.update();
    delay(50);
  }
  M5.update();

  ledGreen;

  // トルクオン
  servo1.TorqueOn();
  servo2.TorqueOn();
  servo3.TorqueOn();
  servo4.TorqueOn();
  servo5.TorqueOn();

  // 0度へ移動
  servo1.Move(0, 10);
  servo2.Move(0, 10);
  servo3.Move(0, 10);
  servo4.Move(0, 10);
  servo5.Move(0, 10);

}

boolean sw = false;
void loop() {
  // ボタン
  if (M5.Btn.wasPressed()) {
    sw = !sw;
    if (sw) {
      ledBlue;
      servo1.Pos(1000);
      servo2.Pos(0);
      servo3.Pos(500);
      servo4.Pos(-400);
      servo5.Pos(800);
    }
    else {
      ledGreen;
      servo1.Pos(100);
      servo2.Pos(1000);
      servo3.Pos(-500);
      servo4.Pos(500);
      servo5.Pos(-800);

    }
  }
  M5.update();
  delay(50);
}

/*-------------------------------------------------
  名前: メモリマップの表示
  機能: DispSerialMap00to29
  引数
    srv: 表示するサーボ
  戻値: なし
  -------------------------------------------------*/
void DispSerialMap00to29(TMFutabaRS *srv) {
  Serial.println("-- Memory map 00-29 ---------------");
  Serial.printf(" Model                  : %s\n", srv->ModelString());
  Serial.printf(" Firmware Version       : %02X\n", srv->FirmwareVersion);
  Serial.printf(" ID                     : %02X\n", srv->ID);
  Serial.printf(" Reverse                : %s\n", srv->isReverse ? "false" : "true");
  Serial.printf(" Baud Rate              : %d bps\n", srv->BaudRate());
  Serial.printf(" Return Delay           : %d usec\n", srv->ReturnDelayMicroSec);
  Serial.printf(" Right Limit Angle      : %-+3.1f deg\n", (float)(srv->LimitAngleR) * 0.1);
  Serial.printf(" Left  Limit Angle      : %-+3.1f deg\n", (float)(srv->LimitAngleL) * 0.1);
  Serial.printf(" Temperature Limit      : %d deg\n", srv->LimitTemp);
  Serial.printf(" Torque In Silence      : %s\n", srv->TorqueInSilenceString());
  Serial.printf(" Warm-up Time           : %1.2f sec\n", (float)(srv->WarmUpTime) * 0.01);
  Serial.printf(" Right Compliance Margin: %2.1f deg\n", (float)(srv->ComplianceMarginR) * 0.1);
  Serial.printf(" Left  Compliance Margin: %2.1f deg\n", (float)(srv->ComplianceMarginL) * 0.1);
  Serial.printf(" Right Compliance Slope : %d deg\n", srv->ComplianceSlopeR);
  Serial.printf(" Left  Compliance Slope : %d deg\n", srv->ComplianceSlopeL);
  Serial.printf(" Punch                  : %3.2f %%\n", (float)(srv->Punch) * 0.01);
  Serial.println("-----------------------------------");
}

/*-------------------------------------------------
  名前: メモリマップの表示
  機能: DispSerialMap30to59
  引数
    srv: 表示するサーボ
  戻値: なし
  -------------------------------------------------*/
void DispSerialMap30to59(TMFutabaRS *srv) {
  Serial.println("-- Memory map 30-59 ---------------");
  Serial.printf(" Goal Position          : %3.1f deg\n", (float)(srv->GoalPosition) * 0.1);
  Serial.printf(" Goal Time              : %d msec\n", srv->GoalTime);
  Serial.printf(" Max Torque             : %d %%\n", srv->MaxTorque);
  Serial.printf(" Torque Mode            : %s\n", srv->TorqueModeString());
  Serial.printf(" Current Position       : %3.1f deg\n", (float)(srv->CurrentPosition) * 0.1);
  Serial.printf(" Current Time           : %d msec\n", srv->CurrentTime * 10);
  Serial.printf(" Current Speed          : %d deg/sec\n", srv->CurrentSpeed);
  Serial.printf(" Current Current        : %d mA\n", srv->CurrentCurrent);
  Serial.printf(" Current Temperature    : %d deg\n", srv->CurrentTemperature);
  Serial.printf(" Current Volts          : %1.2f V\n", srv->CurrentVolts * 0.01);
  Serial.println("-----------------------------------");
}

/*-------------------------------------------------
  名前: メモリマップの表示
  機能: DispSerialMap42to59
  引数
  srv: 表示するサーボ
  戻値: なし
  -------------------------------------------------*/
void DispSerialMap42to59(TMFutabaRS *srv) {
  Serial.println("-- Memory map 42-59 ---------------");
  Serial.printf(" Current Position       : %3.1f deg\n", (float)(srv->CurrentPosition) * 0.1);
  Serial.printf(" Current Time           : %d msec\n", srv->CurrentTime);
  Serial.printf(" Current Speed          : %d deg/sec\n", srv->CurrentSpeed);
  Serial.printf(" Current Current        : %d mA\n", srv->CurrentCurrent);
  Serial.printf(" Current Temperature    : %d deg\n", srv->CurrentTemperature);
  Serial.printf(" Current Volts          : %1.2f V\n", srv->CurrentVolts * 0.01);
  Serial.println("-----------------------------------");
}
