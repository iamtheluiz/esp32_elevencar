#include <WiFi.h> //Inclui a biblioteca
const char* ssid = "ElevenCar"; //Define o nome do ponto de acesso
const char* pass = "cgqp6509"; //Define a senha


//Confugurações dos pinos

int F1 = 13; 
int F2 = 12;
int F3 = 14;
int F4 = 27;
int D1 = 5;
int D2 = 18;
int D3 = 19;
int D4 = 21;
 
WiFiServer sv(80); //Cria um servidor na porta 80
void setup() {
  pinMode(F1, OUTPUT);
  pinMode(F2, OUTPUT);
  pinMode(F3, OUTPUT);
  pinMode(F4, OUTPUT);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  //Deixar o carrinho com as rodas livres
  digitalWrite(F1, LOW);
  digitalWrite(F2, LOW);
  digitalWrite(F3, LOW);
  digitalWrite(F4, LOW);

  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);

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
        if (line.endsWith("GET /up")) { //Se a linha terminar com "/ligar", liga o led
          Serial.println("Tá indo para a frente");          
          digitalWrite(F1, HIGH);
          digitalWrite(F2, LOW);
          digitalWrite(F3, HIGH);
          digitalWrite(F4, LOW);

          digitalWrite(D1, HIGH);
          digitalWrite(D2, LOW);
          digitalWrite(D3, HIGH);
          digitalWrite(D4, LOW);
          delay(100);
        }
        if (line.endsWith("GET /down")) { //Se a linha terminar com "/desligar", desliga o led
          Serial.println("Tá indo para a Tras"); 
          digitalWrite(F1, LOW);
          digitalWrite(F2, HIGH);
          digitalWrite(F3, LOW);
          digitalWrite(F4, HIGH);

          digitalWrite(D1, LOW);
          digitalWrite(D2, HIGH);
          digitalWrite(D3, LOW);
          digitalWrite(D4, HIGH);   
          delay(100);       
        }
        if (line.endsWith("GET /left")) { //Se a linha terminar com "/ligar", liga o led
          Serial.println("Tá indo para esquerda");   
          digitalWrite(F1, HIGH);
          digitalWrite(F2, LOW);
          digitalWrite(F3, LOW);
          digitalWrite(F4, HIGH);

          digitalWrite(D1, HIGH);
          digitalWrite(D2, LOW);
          digitalWrite(D3, HIGH);
          digitalWrite(D4, LOW);      
          delay(100);    
        }
        if (line.endsWith("GET /right")) { //Se a linha terminar com "/desligar", desliga o led
          Serial.println("Tá indo para a direita");  
          digitalWrite(F1, LOW);
          digitalWrite(F2, HIGH);
          digitalWrite(F3, HIGH);
          digitalWrite(F4, LOW);

          digitalWrite(D1, HIGH);
          digitalWrite(D2, LOW);
          digitalWrite(D3, HIGH);
          digitalWrite(D4, LOW);     
          delay(100);       
        }
        if (line.endsWith("GET /stop")) {
          Serial.println("Está parado");  
          digitalWrite(F1, LOW);
          digitalWrite(F2, LOW);
          digitalWrite(F3, LOW);
          digitalWrite(F4, LOW);

          digitalWrite(D1, LOW);
          digitalWrite(D2, LOW);
          digitalWrite(D3, LOW);
          digitalWrite(D4, LOW);     
          delay(100);       
        }
      }
    }
    client.stop(); //Para o cliente
  }
}
