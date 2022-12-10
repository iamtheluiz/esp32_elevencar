#include <WiFi.h> //Inclui a biblioteca
const char* ssid = "lelindinha"; //Define o nome do ponto de acesso
const char* pass = "mnbr6086"; //Define a senha


//Confugurações dos pinos

int F1 = 13; 
int F2 = 12;
int F3 = 14;
int F4 = 27;
int SetaFD = 32;
int FarolFD = 33;
int FreioFD = 22;
int D1 = 5;
int D2 = 4;
int D3 = 19;
int D4 = 21;
int SetaFE = 26;  // Seta
int FarolFE = 25;
int FreioFE = 23;
int Buzina = 18;

// Estados:
// - u => Frente
// - r => Direita
// - l => Esquerda
// - d => Ré
// - s => Parado
char* state = "s"; // u, d, l, r, s
 
WiFiServer sv(80); //Cria um servidor na porta 80
void setup() {
  pinMode(F1, OUTPUT);
  pinMode(F2, OUTPUT);
  pinMode(F3, OUTPUT);
  pinMode(F4, OUTPUT);
  pinMode(SetaFD, OUTPUT);
  pinMode(FarolFD, OUTPUT);
  pinMode(FreioFD, OUTPUT);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(SetaFE, OUTPUT);
  pinMode(FarolFE, OUTPUT);
  pinMode(FreioFE, OUTPUT);

  pinMode(Buzina, OUTPUT);

  //Deixar o carrinho com as rodas livres
  digitalWrite(F1, LOW);
  digitalWrite(F2, LOW);
  digitalWrite(F3, LOW);
  digitalWrite(F4, LOW);
  digitalWrite(SetaFD, LOW);
  digitalWrite(FarolFD, LOW);
  digitalWrite(FreioFD, LOW);

  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(SetaFE, LOW);
  digitalWrite(FarolFE, LOW);
  digitalWrite(FreioFE, LOW);

  digitalWrite(Buzina, LOW);

  Serial.begin(115200); //Inicia o monitor serial
  pinMode(23, OUTPUT);  //Define a porta 23 como saída
  delay(10);  //Atraso de 10 milissegundos
  Serial.println("\n"); //Pula uma linha
  WiFi.begin(ssid, pass); //Inicia o ponto de acesso
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  IPAddress ip = WiFi.localIP(); //Endereço de IP
  
  Serial.print("Endereço de IP: "); //Imprime o endereço de IP
  Serial.println(ip);
  sv.begin(); //Inicia o servidor 
  Serial.println("Servidor online"); //Imprime a mensagem de início
}

void definirEstadoMotor(int pin1, int pin2, int estado) {
  if (estado == 0) {  // Parado
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  } else if (estado == 1) { // Para frente
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  } else if (estado == -1) {  // Para trás
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  }
}

void acenderSeta(int setaPin) {
  delay(300);
  digitalWrite(setaPin, HIGH);
  delay(300);
  digitalWrite(setaPin, LOW);
  
}

void loop() {
  WiFiClient client = sv.available(); //Cria o objeto cliente
  if (client) { //Se este objeto estiver disponível
    String line = ""; //Variável para armazenar os dados recebidos
    Serial.print("Client Connected: ");
    Serial.println(client.connected());
    while (client.connected()) { //Enquanto estiver conectado
      if (client.available()) { //Se estiver disponível
        char c = client.read(); //Lê os caracteres recebidos
        if (c == '\n') { //Se houver uma quebra de linha
          if (line.length() == 0) { //Se a nova linha tiver 0 de tamanho
            break;
          } else {   
            line = "";
          }
        } else if (c != '\r') { 
          line += c; //Adiciona o caractere recebido à linha de leitura
        }
        if (line.endsWith("GET /fon")) { //Se a linha terminar com "/ligar", liga o led
          state = "s";
          Serial.println("Buzina");
          tone(Buzina, 440, 1000);
        }
        if (line.endsWith("GET /up")) { //Se a linha terminar com "/ligar", liga o led
          state = "u";
          Serial.println("Tá indo para a frente");
          definirEstadoMotor(F1, F2, 1);
          definirEstadoMotor(F3, F4, 1);
          definirEstadoMotor(D1, D2, 1);
          definirEstadoMotor(D3, D4, 1);
          delay(100);
        }
        if (line.endsWith("GET /down")) { //Se a linha terminar com "/desligar", desliga o led
          state = "d";
          Serial.println("Tá indo para a Tras");
          definirEstadoMotor(F1, F2, -1);
          definirEstadoMotor(F3, F4, -1);
          definirEstadoMotor(D1, D2, -1);
          definirEstadoMotor(D3, D4, -1);
          delay(100);       
        }
        if (line.endsWith("GET /left")) { //Se a linha terminar com "/ligar", liga o led
          state = "l";
          Serial.println("Tá indo para esquerda");
          definirEstadoMotor(F1, F2, 1);
          definirEstadoMotor(F3, F4, -1);
          definirEstadoMotor(D1, D2, 1);
          definirEstadoMotor(D3, D4, 1);
          delay(100);    
        }
        if (line.endsWith("GET /right")) { //Se a linha terminar com "/desligar", desliga o led
          state = "r";
          Serial.println("Tá indo para a direita");
          definirEstadoMotor(F1, F2, -1);
          definirEstadoMotor(F3, F4, 1);
          definirEstadoMotor(D1, D2, 1);
          definirEstadoMotor(D3, D4, 1);
          delay(100);       
        }
        if (line.endsWith("GET /stop")) {
          state = "s";
          Serial.println("Está parado");
          definirEstadoMotor(F1, F2, 0);
          definirEstadoMotor(F3, F4, 0);
          definirEstadoMotor(D1, D2, 0);
          definirEstadoMotor(D3, D4, 0);
          delay(100);       
        }
      }
    }
    client.stop(); //Para o cliente
  }

  if (state == "r") { // Setas para a direita
    acenderSeta(SetaFD);
  }
  if (state == "l") { // Setas para a esquerda
    acenderSeta(SetaFE);
  }
  if (state == "u") { // Farol da frente
    digitalWrite(FarolFD, HIGH);
    digitalWrite(FarolFE, HIGH);
  }
  if (state == "d") { // Setas para a direita
    digitalWrite(FreioFD, HIGH);
    digitalWrite(FreioFE, HIGH);

    tone(Buzina, 440, 500);
    delay(800);
    noTone(Buzina);
  }
  if (state == "s") { // Setas para a direita
    digitalWrite(FarolFD, LOW);
    digitalWrite(FarolFE, LOW);
    digitalWrite(FreioFD, LOW);
    digitalWrite(FreioFE, LOW);
  }
}