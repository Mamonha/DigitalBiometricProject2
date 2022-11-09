#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 


#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(13, 15);
#else
#define mySerial Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


uint8_t id;
IPAddress server_addr(0,0,0,0);  // IP of the MySQL *server* here
char user[] = "root";              // MySQL user login username
char password[] = "";               // MySQL user login password
char default_db[] = "esp8266db";        // Database Name
char ssid[] = "teste";         // Wifi Id
char pass[] = "teste123";             // Wifi Password


WiFiClient client;                 // Use this for WiFi instead of EthernetClient
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;
LiquidCrystal_I2C lcd(0x27,16,2);


void setup() {
 

  Serial.begin(115200);
  while (!Serial);

  // Begin WiFi section
  
   finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Sensor biométrico encontrado!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
  Serial.printf("\nConectando ao", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 lcd.init();
  // print out info about the connection:
  Serial.println("\nConectou a internet");
  Serial.print("Meu endereço ip: ");
  Serial.println(WiFi.localIP());
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Conectando ao");
  lcd.setCursor(5,1);
  lcd.print("MYSQL");
  delay(1500);
  lcd.clear();
  Serial.print("Conectando ao Banco de dados  ");
    

  if (conn.connect(server_addr, 3306, user, password, default_db))
    lcd.print("Conectado!!");
  else
    Serial.println("Falha");
  
  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);

  Serial.println("Entre 1 para cadastrar novo usuário");
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void cadastro(){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Digite um Nome");
  Serial.println("Entre com o nome do indivíduo:");
 
  //PEGO O USUARIO
  char* username = readUsername();
  Serial.println(username);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Estudante?(y/n)");
  Serial.println("É estudante?(y/n)");
  char bEstudante = readEstudante(); 
  Serial.println(bEstudante);


  
  Serial.println("Pronto para registrar uma impressão digital");
  Serial.println("Por favor, digite um ID # (de 1 a 127) que você deseja salvar");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Digite um numero");
  lcd.setCursor(4,1);
  lcd.print("de 1 a 127");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Id da biometria #");
  Serial.println(id);
  int code = -1;

   while (code != FINGERPRINT_OK){
    
    code = getFingerprintEnroll();
    Serial.print(code);
   
  }
  String bStudie = String(bEstudante);
  String usernamestr = String(username);
  String idstr = String(id);
  String INSERT_SQL = "INSERT INTO usuario(nome,id_user,estudo) VALUES ('"+usernamestr+"',"+idstr+","+bStudie+")";

  if (conn.connected()){
    char charbuf[255];
    INSERT_SQL.toCharArray(charbuf,255);
    cursor->execute(charbuf);

   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Cadastrado com");
   lcd.setCursor(4,1);
   lcd.print("Sucesso");
   Serial.println("Inserido no MYSQL");
    
    delay(3500);
}

  Serial.println("Cadastro concluído!");
  
  Serial.println("Entre 1 para cadastrar novo usuário");
  
}



// ESSA FUNCAO LE O INPUT DO USUARIO

char readEstudante(){
  
  static char bEstudante;
  bEstudante = 0;
  while(Serial.available() != 0){
        Serial.read();
      }
    while(Serial.available() == 0){
      
    delay(500);
   }
    while(Serial.available()>0){

      char inByte = Serial.read();

      if(inByte != '\n'){
      
       if(inByte == 'y'|| inByte == 'Y'){
        bEstudante = '1';
       }
        if(inByte == 'n'|| inByte == 'N'){
        bEstudante = '0';
       }
      
      
      }
    }
    return bEstudante;
 
}
  char* readUsername(){
  
    static char username[50];
    static unsigned int username_pos = 0;
  
     while(Serial.available() != 0){
        Serial.read();
      }
     while(Serial.available() == 0){
      
      delay(500);
     }
      
      while(Serial.available()>0){
    
  
      char inByte = Serial.read();
  
      if(inByte != '\n'){
  
        username[username_pos] = inByte;
        username_pos++;
      }else{
        username[username_pos]= '\0';
  
        username_pos = 0;
  
        return username;
      }
    }
    
  }


void loop() {


   
    if(Serial.available()){
      char comando = Serial.read();
    
      if(comando == '1'){
        cadastro();
      }
    
    } 
    
     getFingerprintID();
     Serial.flush();
     delay(50);  
 
                       
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Aguardando uma biometria válida #"); Serial.println(id);
  lcd.clear();
  lcd.setCursor(3,0); // Vai setar, ou seja, iniciar o cursor na posição 0,0
  lcd.print("Cadastro de");
  lcd.setCursor(4,1);
  lcd.print("Biometria");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Insira o dedo");
  lcd.setCursor(4,1);
  lcd.print("no Sensor");

 
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem coletada");
      break;
      case FINGERPRINT_NOFINGER:
    //  Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Erro na imagem");
      break;
    default:
      //Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagem bagunçada");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Recursos da impressão não encontradas");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Imagem não encontrada");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Remova o dedo");
  Serial.println("Remova o dedo");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Coloque o mesmo");
  lcd.setCursor(4,1);
  lcd.print("dedo");
  Serial.println("Coloque o mesmo dedo novamente");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem coletada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Erro na imagem");
      break;
    default:
      //Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagem bagunçada");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Recursos da impressão não encontradas");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Imagem não encontrada");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Criando um modelo para #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Impressão correspondente!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro de comunicação");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("As impressões não coincidem");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Biometria coletada!");
    Serial.println("Cadastro realizado com sucesso!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Não foi possível armazenar nesse local");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Erro ao gravar no flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return FINGERPRINT_OK;
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem encontrada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Nenhum dedo encontrado");
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Coloque o dedo");
      lcd.setCursor(3,1);
      lcd.print("no Sensor");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;

  }
  
   p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagem bagunçada");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Recursos da impressão não encontradas");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Imagem não encontrada");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

   p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Impressão digital encontrada!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Não encontrou biometria válida");
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Biometria nao");
    lcd.setCursor(3,1);
    lcd.print("Registrada");
    
    delay(2000);
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  
  String idedo;
  idedo = String(finger.fingerID);
  String SELECT_SQL = "SELECT nome,estudo FROM usuario WHERE id_user="+idedo;
  

  Serial.println(SELECT_SQL);
  char charbuf2[255];
  char charbuf3[255];
 
  SELECT_SQL.toCharArray(charbuf2,255);
    cursor->execute(charbuf2);
    Serial.println(); 

    column_names *columns = cursor->get_columns();

  
  String nome3;
  int saldo;
  String estudo;
  //int saldo;
  row_values *row = NULL;
  do {
    row = cursor->get_next_row();
    if (row != NULL) {
  
      nome3 = (row->values[0]);
      estudo =(row->values[1]);
     
    }
  } while (row != NULL);
  // Deleting the cursor also frees up memory used
  Serial.print("Usuário encontrado id: #"); Serial.print(finger.fingerID);

  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Bem vindo");
  lcd.setCursor(3,1);
  lcd.print(nome3);
  delay(3000);
  lcd.clear();
  String UPDATE_SQL;
  if(estudo == "1"){

   UPDATE_SQL = "UPDATE usuario SET saldo = saldo - 2, tempo=now() WHERE id_user="+idedo;
  }else{
    UPDATE_SQL = "UPDATE usuario SET saldo = saldo - 5, tempo=now() WHERE id_user="+idedo;
  }
 

   UPDATE_SQL.toCharArray(charbuf3,255);
   cursor->execute(charbuf3);
   Serial.println();
   
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  
  return finger.fingerID;

 
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  
  Serial.print(" with confidence of "); Serial.println(finger.confidence);


  return finger.fingerID;
}
