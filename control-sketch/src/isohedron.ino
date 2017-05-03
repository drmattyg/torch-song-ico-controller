#include <CurieIMU.h>
#include <CurieBLE.h>
BLEPeripheral     blePeripheral;
BLEService        torchSongCtl("180D");
BLECharacteristic torchSongFace("2A37",BLERead|BLENotify,sizeof(int));

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  CurieIMU.begin();
  CurieIMU.setAccelerometerRange(2);
  blePeripheral.setLocalName("touch-song-control");
  blePeripheral.setAdvertisedServiceUuid(torchSongCtl.uuid());
  blePeripheral.addAttribute(torchSongCtl);
  blePeripheral.addAttribute(torchSongFace);
  blePeripheral.begin();
}

// icosahedron face normal vectors
int32_t normal[20][3] = {
  { 0 , 0 , 3464 },
  { 1825 , -1414 , 2581 },
  { 311 , 2288 , 2581 },
  { -2137 , -874 , 2581 },
  { 2330 , 2288 , 1154 },
  { 3265 , 0 , 1154 },
  { -1632 , -2828 , 1154 },
  { -3146 , 874 , 1154 },
  { -1632 , 2828 , 1154 },
  { 816 , -3162 , 1154 },
  { 1632 , -2828 , -1154 },
  { 1632 , 2828 , -1154 },
  { -3265 , 0 , -1154 },
  { -2330 , -2288 , -1154 },
  { -816 , 3162 , -1154 },
  { 3146 , -874 , -1154 },
  { -311 , -2288 , -2581 },
  { -1825 , 1414 , -2581 },
  { 2137 , 874 , -2581 },
  { 0 , 0 , -3464 }
};

int16_t X,  Y,  Z;

// Comput unnormalized dot product.
int dotprod(int k)
{
  return X*normal[k][0]+Y*normal[k][1]+Z*normal[k][2];
}

// Find the most up face
int faceup()
{
  int maxval(dotprod(0));
  int tmpval(0);
  int face(0);
  for (int k = 1; k < 20; k++) {
    tmpval = dotprod(k);
    if ( tmpval > maxval) {
      face = k;
      maxval = tmpval;
    }
  }
  return face;
}

int upface(-1);

void loop()
{
  int face;
  BLECentral central = blePeripheral.central();
  CurieIMU.getAcceleration(&X,&Y,&Z);
  face = faceup();
  if (face != upface) {
    upface = face;
    Serial.print(upface);
    Serial.println();
  }
  if (central) {
    Serial.println("Shoob");
    torchSongFace.setValue((unsigned char*)&upface,sizeof(int));
  }
}
